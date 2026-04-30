#include "hive/plugins/dictionary/triggers/InsteadOfReadListDictionaryTermMetricsTrigger.hpp"
#include "hive/essential/Global.hpp"
#include "hive/api/AccessTokenContext.hpp"

#include "../../../../../../include/hive-db-sqlite/hive/db/sqlite/queries/dictionary/FindDictionaryTermMetricsSQLiteQuery.hpp"
#include "hive/plugins/dictionary/models/DictionaryTerm.hpp"

#include "hive/plugins/dictionary/models/DictionaryTermMetric.hpp"
#include "hive/util/Utils.hpp"

namespace hive::plugins::dictionary::triggers
{
    using_loggers()

    InsteadOfReadListDictionaryTermMetricsTrigger::InsteadOfReadListDictionaryTermMetricsTrigger()
        : Trigger(
            "InsteadOfListDictionaryTermMetricsTrigger",
            "Calls custom sql for list term metric request",
            1000,
            { essential::Crudl::Read, essential::Crudl::List },
            api::TriggerPhase::InsteadOf,
            "dictionary_term_metric"
        )
    {
    }

        std::optional<std::pair<entity_fields, api::OperationResult>> InsteadOfReadListDictionaryTermMetricsTrigger::
    run_instead_of_read(int stack_depth, api::OperationResult& validation_result, const model::ModelDefinition& def,
                        identification user_id, identification id)
    {
        api::AccessTokenContext ctx{user_id, "", 200};
        models::DictionaryTermMetric result;

        auto read_term = run_read(models::DICTIONARY_TERM_DEFINITION, ctx, id, stack_depth);
        if (!read_term.second)
        {
            auto values = result.to_values();
            auto r = std::make_pair<entity_fields, api::OperationResult>(
                std::move(values),
                {500, "Internal server error."});
            return r;
        }
        models::DictionaryTerm term;
        term.from_values(read_term.first);

        orm::QueryParams qp;
        qp.add_filter("dictionary_map_id", term.dictionary_map_id);
        qp.add_filter("dictionary_term_id", term.get_id());

        auto list_metrics = run_list(models::DICTIONARY_TERM_METRIC_DEFINITION, ctx, qp, stack_depth);
        if (!list_metrics.second)
        {
            auto values = result.to_values();
            auto r = std::make_pair<entity_fields, api::OperationResult>(
                std::move(values),
                {500, "Internal server error."});
            return r;
        }

        if (list_metrics.first.empty())
        {
            auto values = result.to_values();
            auto r = std::make_pair<entity_fields, api::OperationResult>(
                std::move(values),
                {500, "Internal server error."});
            return r;
        }
        result.from_values(list_metrics.first[0]);

        auto v = result.to_values();
        auto r =
            std::make_pair<entity_fields, api::OperationResult>
            (std::move(v), ok_result);

        return r;
    }

    std::optional<std::pair<std::vector<entity_fields>, api::OperationResult>>
    InsteadOfReadListDictionaryTermMetricsTrigger::run_instead_of_list(
        int /*stack_depth*/,
        api::OperationResult& /*validation_result*/,
        const model::ModelDefinition& /*def*/,
        identification user_id,
        const orm::QueryParams& query_params)
    {
        if (user_id == 0)
        {
            return std::make_pair(
                std::vector<entity_fields>{},
                api::OperationResult{500, "Internal server error"});
        }

        nlohmann::json req;

        std::vector<entity_fields> v;
        std::optional<std::pair<std::vector<entity_fields>, api::OperationResult>> result;

        // --- mandatory ---
        auto dictionary_map_id = query_params.filters.at("dictionary_map_id");
        req["dictionary_map_id"] = std::stoll(dictionary_map_id);
        req["user_id"] = user_id;

        // --- optional ---
        if (query_params.filters.contains("dictionary_term_id"))
        {
            req["dictionary_term_id"] =
                std::stoll(query_params.filters.at("dictionary_term_id"));
        }

        req["page_size"]   = query_params.page_size;
        req["page_number"] = query_params.page_number;

        nlohmann::json res;

        try
        {
            res = call_query(
                db::sqlite::queries::dictionary::QUERY_FindDictionaryTermMetrics,
                req);

            if (res.contains("error"))
            {
                err << res.dump() << commit;
                return std::make_pair(
                    std::vector<entity_fields>{},
                    api::OperationResult{500, "Internal server error"});
            }
        }
        catch (const std::exception& e)
        {
            err << "FindDictionaryTermMetrics failed: " << e.what() << commit;
            return std::make_pair(
                std::vector<entity_fields>{},
                api::OperationResult{500, "Internal server error"});
        }

        for (const auto& e : res["results"])
        {
            models::DictionaryTermMetric m;

            m.set_id(e.at("dictionary_term_id").get<int64_t>());
            m.dictionary_term_id = e.at("dictionary_term_id").get<int64_t>();
            m.dictionary_map_id  = e.at("dictionary_map_id").get<int64_t>();

            m.tag_count        = e.value("tag_count", 0);
            m.flag_count       = e.value("flag_count", 0);
            m.alias_count      = e.value("alias_count", 0);
            m.note_count       = e.value("note_count", 0);

            m.link_count       = e.value("link_count", 0);
            m.backlink_count   = e.value("backlink_count", 0);

            m.url_count        = e.value("url_count", 0);
            m.source_count     = e.value("source_count", 0);
            m.index_count      = e.value("index_count", 0);

            m.review_count     = e.value("review_count", 0);
            m.state_18_count   = e.value("state_18_count", 0);

            m.update_count     = e.value("update_count", 0);
            m.view_count       = e.value("view_count", 0);
            unixtime last_viewed_at = e.value("last_viewed_at", 0L);
            m.last_viewed_at   = last_viewed_at;
            unixtime last_updated_at = e.value("last_updated_at", 0L);
            m.last_updated_at   = last_updated_at;

            auto values = m.to_values();
            int64_t now = static_cast<int64_t>(util::Utils::current_unix_timestamp_ms());
            values[1] = now;
            values[2] = now;
            v.push_back(values);
        }
        result = std::make_pair<std::vector<entity_fields>, api::OperationResult>(std::move(v), ok_result);
        return result;
    }
}
