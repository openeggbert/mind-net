#include "../../../../../../../include/hive-db-sqlite/hive/db/sqlite/queries/dictionary/FindDictionaryTermMetricsSQLiteQuery.hpp"
#include <SQLiteCpp/SQLiteCpp.h>

#include "hive/db/sqlite/SqliteFileName.hpp"
#include "hive/essential/DatabaseType.hpp"
#include "hive/essential/Global.hpp"

namespace hive::db::sqlite::queries::dictionary
{
    FindDictionaryTermMetricsSQLiteQuery::FindDictionaryTermMetricsSQLiteQuery()
        : Query(
            QUERY_FindDictionaryTermMetrics,
            "FindDictionaryTermMetricsSQLiteQuery",
            essential::DatabaseType::SQLite)
    {
    }

    nlohmann::json FindDictionaryTermMetricsSQLiteQuery::call(
        nlohmann::json& request,
        api::InvalidateMethod& /*invalidate_method*/,
        plugins::core::models::OptionalError& /*optional_error*/)
    {
        nlohmann::json response;

        if (!request.contains("dictionary_map_id"))
            throw std::invalid_argument("Mandatory key dictionary_map_id is missing");

        if (!request.contains("user_id"))
            throw std::invalid_argument("Mandatory key user_id is missing");

        identification dictionary_map_id = request["dictionary_map_id"];
        identification user_id           = request["user_id"];
        bool any_map = dictionary_map_id == 0;

        identification dictionary_term_id{0};
        if (request.contains("dictionary_term_id"))
            dictionary_term_id = request["dictionary_term_id"];

        int page_size   = request.value("page_size", 10);
        int page_number = request.value("page_number", 1);

        static const std::string sql_base = R"(
SELECT
    t.id AS dictionary_term_id,
    t.dictionary_map_id AS dictionary_map_id,

    COALESCE(tag_cnt.cnt, 0)        AS tag_count,
    COALESCE(flag_cnt.cnt, 0)       AS flag_count,
    COALESCE(alias_cnt.cnt, 0)      AS alias_count,
    COALESCE(note_cnt.cnt, 0)       AS note_count,

    COALESCE(link_cnt.cnt, 0)       AS link_count,
    COALESCE(backlink_cnt.cnt, 0)   AS backlink_count,

    COALESCE(url_cnt.cnt, 0)        AS url_count,
    COALESCE(source_cnt.cnt, 0)     AS source_count,
    COALESCE(index_cnt.cnt, 0)      AS index_count,

    COALESCE(review_cnt.cnt, 0)     AS review_count,
    COALESCE(state18_cnt.cnt, 0)    AS state_18_count,

    COALESCE(update_cnt.cnt, 0)     AS update_count,
    update_cnt.last_updated_at     AS last_updated_at,
    COALESCE(visit_cnt.cnt, 0)      AS view_count,
    visit_cnt.last_viewed_at        AS last_viewed_at

FROM dictionary_term t

LEFT JOIN (
    SELECT dictionary_term_id, COUNT(*) cnt
    FROM dictionary_tag
    GROUP BY dictionary_term_id
) tag_cnt ON tag_cnt.dictionary_term_id = t.id

LEFT JOIN (
    SELECT dictionary_term_id, COUNT(*) cnt
    FROM dictionary_flag
    GROUP BY dictionary_term_id
) flag_cnt ON flag_cnt.dictionary_term_id = t.id

LEFT JOIN (
    SELECT dictionary_term_id, COUNT(*) cnt
    FROM dictionary_term_alias
    GROUP BY dictionary_term_id
) alias_cnt ON alias_cnt.dictionary_term_id = t.id

LEFT JOIN (
    SELECT dictionary_term_id, COUNT(*) cnt
    FROM dictionary_note
    GROUP BY dictionary_term_id
) note_cnt ON note_cnt.dictionary_term_id = t.id

LEFT JOIN (
    SELECT from_dictionary_term_id AS term_id, COUNT(*) cnt
    FROM dictionary_link
    GROUP BY from_dictionary_term_id
) link_cnt ON link_cnt.term_id = t.id

LEFT JOIN (
    SELECT to_dictionary_term_id AS term_id, COUNT(*) cnt
    FROM dictionary_link
    GROUP BY to_dictionary_term_id
) backlink_cnt ON backlink_cnt.term_id = t.id

LEFT JOIN (
    SELECT dictionary_term_id, COUNT(*) cnt
    FROM dictionary_url
    GROUP BY dictionary_term_id
) url_cnt ON url_cnt.dictionary_term_id = t.id

LEFT JOIN (
    SELECT dictionary_term_id, COUNT(*) cnt
    FROM dictionary_source
    GROUP BY dictionary_term_id
) source_cnt ON source_cnt.dictionary_term_id = t.id

LEFT JOIN (
    SELECT dictionary_term_id, COUNT(*) cnt
    FROM dictionary_index
    GROUP BY dictionary_term_id
) index_cnt ON index_cnt.dictionary_term_id = t.id

LEFT JOIN (
    SELECT dictionary_term_id, COUNT(*) cnt
    FROM dictionary_review
    WHERE user_id = ?
    GROUP BY dictionary_term_id
) review_cnt ON review_cnt.dictionary_term_id = t.id

LEFT JOIN (
    SELECT dictionary_term_id, COUNT(*) cnt
    FROM dictionary_state_18
    WHERE user_id = ?
    GROUP BY dictionary_term_id
) state18_cnt ON state18_cnt.dictionary_term_id = t.id

LEFT JOIN (
    SELECT
        record_id AS dictionary_term_id,
        COUNT(*) AS cnt,
        MAX(created_at) AS last_updated_at
    FROM history
    WHERE table_name = 'dictionary_term'
      AND operation = 3
    GROUP BY record_id
) update_cnt
ON update_cnt.dictionary_term_id = t.id



LEFT JOIN (
    SELECT dictionary_term_id,
           COUNT(*) cnt,
           MAX(created_at) last_viewed_at
    FROM dictionary_term_visit
    WHERE user_id = ?
    GROUP BY dictionary_term_id
) visit_cnt ON visit_cnt.dictionary_term_id = t.id

WHERE (? = 1 OR t.dictionary_map_id = ?)
)";

        bool term_id_present = dictionary_term_id != 0;
        std::string sql =
            sql_base +
            (term_id_present ? " AND t.id = ? " : "") +
            " LIMIT ? OFFSET ?";

        try
        {
            SQLite::Database db(SQLITE_FILE_NAME, SQLite::OPEN_READONLY);
            db.exec("PRAGMA foreign_keys = ON;");
            db.exec("PRAGMA journal_mode=WAL;");

            SQLite::Statement query(db, sql);

            int i = 0;
            query.bind(++i, user_id); // review
            query.bind(++i, user_id); // state_18
            query.bind(++i, user_id); // visit
            query.bind(++i, any_map ? 1 : 0);
            query.bind(++i, dictionary_map_id);

            if (term_id_present)
                query.bind(++i, dictionary_term_id);

            query.bind(++i, page_size);
            query.bind(++i, (page_number - 1) * page_size);

            std::vector<nlohmann::json> results;

            while (query.executeStep())
            {
                nlohmann::json r;

                r["dictionary_term_id"] = query.getColumn("dictionary_term_id").getInt64();
                r["dictionary_map_id"]  = query.getColumn("dictionary_map_id").getInt64();

                r["tag_count"]        = query.getColumn("tag_count").getInt();
                r["flag_count"]       = query.getColumn("flag_count").getInt();
                r["alias_count"]      = query.getColumn("alias_count").getInt();
                r["note_count"]       = query.getColumn("note_count").getInt();
                r["link_count"]       = query.getColumn("link_count").getInt();
                r["backlink_count"]   = query.getColumn("backlink_count").getInt();
                r["url_count"]        = query.getColumn("url_count").getInt();
                r["source_count"]     = query.getColumn("source_count").getInt();
                r["index_count"]      = query.getColumn("index_count").getInt();
                r["review_count"]     = query.getColumn("review_count").getInt();
                r["state_18_count"]   = query.getColumn("state_18_count").getInt();
                r["update_count"]     = query.getColumn("update_count").getInt();
                r["last_updated_at"] = query.getColumn("last_updated_at").isNull() ?
                0 : query.getColumn("last_updated_at").getInt64();

                r["view_count"]       = query.getColumn("view_count").getInt();
                r["last_viewed_at"]   = query.getColumn("last_viewed_at").isNull()
                                          ? 0
                                          : query.getColumn("last_viewed_at").getInt64();

                results.push_back(r);
            }

            response["results"] = results;
        }
        catch (SQLite::Exception& e)
        {
            response["error"]      = e.what();
            response["sql_failed"] = sql;
        }

        return response;
    }
}
