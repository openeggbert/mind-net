/*
 * MIT License
 * Copyright (c) 2025 Robert Vokac
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include "mindnet/plugins/dictionary/triggers/InsteadOfListDictionaryTermSearchTrigger.hpp"
#include "mindnet/essential/Global.hpp"
#include "mindnet/api/AccessTokenContext.hpp"
#include <string>
#include <vector>
#include "mindnet/db/sqlite/queries/dictionary/FindDictionaryTermsViaAdvancedSearchSQLiteQuery.hpp"

#include "mindnet/util/Utils.hpp"

namespace mindnet::plugins::dictionary::triggers
{
    using_loggers()

    InsteadOfListDictionaryTermSearchTrigger::InsteadOfListDictionaryTermSearchTrigger()
        : Trigger(
            "InsteadOfListDictionaryTermSearchTrigger",
            "Calls custom sql for list term search request",
            1000,
            {essential::Crudl::List},
            api::TriggerPhase::InsteadOf,
            "dictionary_term_search"
        )
    {
    }

    std::optional<std::pair<std::vector<entity_fields>, api::OperationResult>>
    InsteadOfListDictionaryTermSearchTrigger::run_instead_of_list(
        int stack_depth,
        api::OperationResult& validation_result,
        const model::ModelDefinition& def,
        identification user_id,
        const orm::QueryParams& query_params)
    {
        std::vector<entity_fields> v;
        std::optional<std::pair<std::vector<entity_fields>, api::OperationResult>> result;

        nlohmann::json req;

        auto dictionary_map_id = std::stoll(query_params.filters.at("dictionary_map_id"));
        auto query_json = query_params.filters.at("title");

        req["dictionary_map_id"] = dictionary_map_id;
        req["user_id"] = user_id;
        req["query_json"] = query_json;

        req["page_size"] = query_params.page_size;
        req["page_number"] = query_params.page_number;

        nlohmann::json res;

        try
        {
            res = call_query(
                db::sqlite::queries::dictionary::QUERY_FindDictionaryTermsViaAdvancedSearch,
                req
            );

            if (res.contains("error"))
            {
                err << res.dump() << commit;
                return std::make_pair(std::vector<entity_fields>{},
                                      api::OperationResult{500, std::string("Internal server error ") + res["error"].get<std::string>()});
            }

            int total_items = res["total_items"].get<i64>();
            query_params.total_items = total_items;
            for (const auto& row : res["results"])
            {
                models::DictionaryTermSearch model;

                model.set_id(row["id"].get<identification>());
                model.dictionary_term_id = model.get_id();
                model.dictionary_map_id = dictionary_map_id;

                std::string title = row["title"].get<std::string>();
                std::string dis = row["disambiguation"].get<std::string>();

                model.title = title;
                model.disambiguation = dis;

                // if (!dis.empty())
                //     model.title += " (" + dis + ")";

                auto values = model.to_values();

                int64_t now = static_cast<int64_t>(util::Utils::current_unix_timestamp_ms());
                values[1] = now; // created_at
                values[2] = now; // updated_at

                v.push_back(std::move(values));
            }
        }
        catch (const std::exception& e)
        {
            err << "DictionaryTermSearch failed: " << e.what() << commit;
            return std::make_pair(std::vector<entity_fields>{},
                                  api::OperationResult{500, "Internal server error"});
        }

        result = std::make_pair<std::vector<entity_fields>, api::OperationResult>(std::move(v), ok_result);
        return result;
    }
}
