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

#include "hive/plugins/dictionary/triggers/InsteadOfListDictionaryIndexTypeFulltextTrigger.hpp"
#include "hive/essential/Global.hpp"
#include "hive/api/AccessTokenContext.hpp"
#include <string>
#include <vector>
#include "../../../../../../include/hive-db-sqlite/hive/db/sqlite/queries/dictionary/FindDictionaryIndexTypesSQLiteQuery.hpp"
#include "hive/plugins/dictionary/models/DictionaryIndexTypeFulltext.hpp"

#include "hive/util/Utils.hpp"

namespace hive::plugins::dictionary::triggers
{
    using_loggers()

    InsteadOfListDictionaryIndexTypeFulltextTrigger::InsteadOfListDictionaryIndexTypeFulltextTrigger()
        : Trigger(
            "InsteadOfListDictionaryIndexTypeFulltextTrigger",
            "Calls custom sql for list index_type request",
            1000,
            {essential::Crudl::List},
            api::TriggerPhase::InsteadOf,
            "dictionary_index_type_fulltext"
        )
    {
    }

    std::optional<std::pair<std::vector<entity_fields>, api::OperationResult>>  InsteadOfListDictionaryIndexTypeFulltextTrigger::
    run_instead_of_list(
            int stack_depth,
            api::OperationResult& validation_result,
            const model::ModelDefinition& def,
            identification user_id,
            const orm::QueryParams& query_params)
    {
        std::vector<entity_fields> v;
        std::optional<std::pair<std::vector<entity_fields>, api::OperationResult>> result;

        nlohmann::json req;
        auto dictionary_map_id = query_params.filters.at("dictionary_map_id");
        req["dictionary_map_id"] = std::stoll(dictionary_map_id);
        auto title_part = query_params.filters.at("title_part");
        req["title_part"] = title_part;
        int page_size = query_params.page_size;
        int page_number = query_params.page_number;
        req["page_size"] = page_size;
        req["page_number"] = page_number;
        nlohmann::json res;
        std::vector<std::pair<identification, std::string>> results;

        try
        {
            res = call_query(db::sqlite::queries::dictionary::QUERY_FindDictionaryIndexTypes, req);

            if (res.contains("error"))
            {
                std::vector<entity_fields> v0;
                result = std::make_pair<std::vector<entity_fields>, api::OperationResult>(std::move(v0), {500, "Internal server error."});
                return result;
            }

            results = res["results"];
            info << res.dump() << commit;
            info << "Query FindDictionaryTags successful" << commit;
        }
        catch (std::exception& e)
        {
            err << "Query FindDictionaryTags failed " << e.what() << commit;
            std::vector<entity_fields> v0;
            result = std::make_pair<std::vector<entity_fields>, api::OperationResult>(std::move(v0), {500, "Internal server error."});
            return result;
        }

        for (auto& e:results)
        {
            models::DictionaryIndexTypeFulltext index_type_fulltext;
            index_type_fulltext.set_id(e.first);
            index_type_fulltext.dictionary_index_type_id = e.first;
            index_type_fulltext.dictionary_map_id = std::stoll(dictionary_map_id);
            index_type_fulltext.title_part = title_part;
            index_type_fulltext.title = e.second;
            auto values = index_type_fulltext.to_values();
            int64_t now = static_cast<int64_t>(util::Utils::current_unix_timestamp_ms());
            values[1] = now;
            values[2] = now;
            v.push_back(values);
        }
        result = std::make_pair<std::vector<entity_fields>, api::OperationResult>(std::move(v), ok_result);
        return result;
    }
}