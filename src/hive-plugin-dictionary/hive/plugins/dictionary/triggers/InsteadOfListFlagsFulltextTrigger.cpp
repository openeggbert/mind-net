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

#include "hive/plugins/dictionary/triggers/InsteadOfListDictionaryFlagsFulltextTrigger.hpp"
#include "hive/essential/Global.hpp"
#include "hive/api/AccessTokenContext.hpp"
#include <string>
#include <vector>
#include "../../../../../../include/hive-db-sqlite/hive/db/sqlite/queries/dictionary/FindDictionaryFlagsSQLiteQuery.hpp"
#include "hive/plugins/dictionary/models/DictionaryFlagFulltext.hpp"

#include "hive/util/Utils.hpp"

namespace hive::plugins::dictionary::triggers
{
    using_loggers()

    InsteadOfListDictionaryFlagsFulltextTrigger::InsteadOfListDictionaryFlagsFulltextTrigger()
        : Trigger(
            "InsteadOfListDictionaryFlagsFulltextTrigger",
            "Calls custom sql for list flag fulltext request",
            1000,
            {essential::Crudl::List},
            api::TriggerPhase::InsteadOf,
            "dictionary_flag_fulltext"
        )
    {
    }

    std::optional<std::pair<std::vector<entity_fields>, api::OperationResult>>  InsteadOfListDictionaryFlagsFulltextTrigger::
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
        req["user_id"] = user_id;
        int page_size = query_params.page_size;
        int page_number = query_params.page_number;
        req["page_size"] = page_size;
        req["page_number"] = page_number;
        nlohmann::json res;
        std::vector<std::string> results;

        try
        {
            res = call_query(db::sqlite::queries::dictionary::QUERY_FindDictionaryFlags, req);

            if (res.contains("error"))
            {
                std::vector<entity_fields> v0;
                result = std::make_pair<std::vector<entity_fields>, api::OperationResult>(std::move(v0), {500, "Internal server error."});
                return result;
            }

            results = res["results"];
            info << res.dump() << commit;
            info << "Query FindDictionaryTerms successful" << commit;
        }
        catch (std::exception& e)
        {
            err << "Query FindDictionaryTerms failed " << e.what() << commit;
            std::vector<entity_fields> v0;
            result = std::make_pair<std::vector<entity_fields>, api::OperationResult>(std::move(v0), {500, "Internal server error."});
            return result;
        }

        identification tmp_id{0};
        for (auto& e:results)
        {
            tmp_id++;
            models::DictionaryFlagFulltext flag_fulltext;
            flag_fulltext.set_id(tmp_id);
            flag_fulltext.dictionary_map_id = std::stoll(dictionary_map_id);
            flag_fulltext.title_part = title_part;
            flag_fulltext.title = e;
            auto values = flag_fulltext.to_values();
            int64_t now = static_cast<int64_t>(util::Utils::current_unix_timestamp_ms());
            values[1] = now;
            values[2] = now;
            v.push_back(values);
        }
        result = std::make_pair<std::vector<entity_fields>, api::OperationResult>(std::move(v), ok_result);
        return result;
    }
}