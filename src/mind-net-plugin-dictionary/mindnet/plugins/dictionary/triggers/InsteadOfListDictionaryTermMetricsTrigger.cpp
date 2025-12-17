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

#include "mindnet/plugins/dictionary/triggers/InsteadOfListDictionaryTermMetricsTrigger.hpp"
#include "mindnet/essential/Global.hpp"
#include "mindnet/api/AccessTokenContext.hpp"
#include <string>
#include <vector>
#include "../../../../../../include/mind-net-db-sqlite/mindnet/db/sqlite/queries/dictionary/FindDictionaryTermMetricsSQLiteQuery.hpp"
#include "mindnet/plugins/dictionary/models/DictionaryTermMetric.hpp"

#include "mindnet/util/Utils.hpp"

namespace mindnet::plugins::dictionary::triggers
{
    using_loggers()

    InsteadOfListDictionaryTermMetricsTrigger::InsteadOfListDictionaryTermMetricsTrigger()
        : Trigger(
            "InsteadOfListDictionaryTermMetricsTrigger",
            "Calls custom sql for list term metric request",
            1000,
            {essential::Crudl::List},
            api::TriggerPhase::InsteadOf,
            "dictionary_term_metric"
        )
    {
    }

    std::optional<std::pair<std::vector<entity_fields>, api::OperationResult>>  InsteadOfListDictionaryTermMetricsTrigger::
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

        identification dictionary_term_id{0};
        if (query_params.filters.contains("dictionary_term_id"))
        {
            dictionary_term_id = std::stoll(query_params.filters.at("dictionary_term_id"));
            req["dictionary_term_id"] = dictionary_term_id;
        }

        req["user_id"] = user_id;

        int page_size = query_params.page_size;
        int page_number = query_params.page_number;
        req["page_size"] = page_size;
        req["page_number"] = page_number;
        nlohmann::json res;
        std::vector<nlohmann::json> results;

        try
        {
            res = call_query(db::sqlite::queries::dictionary::QUERY_FindDictionaryTermMetrics, req);

            if (res.contains("error"))
            {
                std::vector<entity_fields> v0;
                result = std::make_pair<std::vector<entity_fields>, api::OperationResult>(std::move(v0), {500, "Internal server error."});
                return result;
            }

            results = res["results"];
            info << res.dump() << commit;
            info << "Query FindDictionaryTermMetrics successful" << commit;
        }
        catch (std::exception& e)
        {
            err << "Query FindDictionaryTermMetrics failed " << e.what() << commit;
            std::vector<entity_fields> v0;
            result = std::make_pair<std::vector<entity_fields>, api::OperationResult>(std::move(v0), {500, "Internal server error."});
            return result;
        }

        for (auto& e:results)
        {
            models::DictionaryTermMetric metric;
            metric.set_id(e["dictionary_term_id"]);
            metric.dictionary_term_id=e["dictionary_term_id"];
            metric.dictionary_map_id=e["dictionary_map_id"];
            metric.tag_count=e["tag_count"];
            metric.flag_count=e["flag_count"];
            metric.link_count=e["link_count"];
            metric.note_count=e["note_count"];
            metric.index_count=e["index_count"];
            metric.source_count=e["source_count"];
            metric.alias_count=e["alias_count"];
            metric.view_count =e["view_count"];
            metric.last_viewed_at=e["last_viewed_at"];

            auto values = metric.to_values();
            int64_t now = static_cast<int64_t>(util::Utils::current_unix_timestamp_ms());
            values[1] = now;
            values[2] = now;
            v.push_back(values);
        }
        result = std::make_pair<std::vector<entity_fields>, api::OperationResult>(std::move(v), ok_result);
        return result;
    }
}