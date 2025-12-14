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

#include "mindnet/plugins/dictionary/triggers/InsteadOfListDictionarySourceTypeFulltextTrigger.hpp"
#include "mindnet/essential/Global.hpp"
#include "mindnet/api/AccessTokenContext.hpp"
#include <string>
#include <vector>
#include "../../../../../../include/mind-net-db-sqlite/mindnet/db/sqlite/queries/FindDictionarySourceTypesSQLiteQuery.hpp"
#include "mindnet/plugins/dictionary/models/DictionarySourceTypeFulltext.hpp"

#include "mindnet/util/Utils.hpp"

namespace mindnet::plugins::dictionary::triggers
{
    using_loggers()

    InsteadOfListDictionarySourceTypeFulltextTrigger::InsteadOfListDictionarySourceTypeFulltextTrigger()
        : Trigger(
            "InsteadOfListDictionarySourceTypeFulltextTrigger",
            "Calls custom sql for list source_type request",
            1000,
            {essential::Crudl::List},
            api::TriggerPhase::InsteadOf,
            "dictionary_source_type_fulltext"
        )
    {
    }

    std::optional<std::pair<std::vector<entity_fields>, api::OperationResult>>  InsteadOfListDictionarySourceTypeFulltextTrigger::
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
            res = call_query(db::sqlite::queries::QUERY_FindDictionarySourceTypes, req);

            if (res.contains("error"))
            {
                std::vector<entity_fields> v0;
                std::string error = res["error"];
                result = std::make_pair<std::vector<entity_fields>, api::OperationResult>(std::move(v0), {500, std::string("Internal server error. ") + error});
                return result;
            }

            results = res["results"];
            info << res.dump() << commit;
            info << "Query FindDictionarySources successful" << commit;
        }
        catch (std::exception& e)
        {
            err << "Query FindDictionarySources failed " << e.what() << commit;
            std::vector<entity_fields> v0;
            result = std::make_pair<std::vector<entity_fields>, api::OperationResult>(std::move(v0), {500, "Internal server error."});
            return result;
        }

        for (auto& e:results)
        {
            models::DictionarySourceTypeFulltext source_type_fulltext;
            source_type_fulltext.set_id(e.first);
            source_type_fulltext.dictionary_source_type_id = e.first;
            source_type_fulltext.title = e.second;
            source_type_fulltext.title_part = title_part;

            auto values = source_type_fulltext.to_values();
            int64_t now = static_cast<int64_t>(util::Utils::current_unix_timestamp_ms());
            values[1] = now;
            values[2] = now;
            v.push_back(values);
        }
        result = std::make_pair<std::vector<entity_fields>, api::OperationResult>(std::move(v), ok_result);
        return result;
    }
}