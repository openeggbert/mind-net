/*
 * MIT License
 * Copyright (c) 2025 Robert Vokac
 */

#include "../../../../../../../include/hive-db-sqlite/hive/db/sqlite/queries/dictionary/FindDictionaryTermsSQLiteQuery.hpp"
#include <SQLiteCpp/SQLiteCpp.h>
#include "hive/db/sqlite/SqliteFileName.hpp"
#include "hive/essential/DatabaseType.hpp"
#include "hive/essential/Global.hpp"

namespace hive::db::sqlite::queries::dictionary
{
    FindDictionaryTermsSQLiteQuery::FindDictionaryTermsSQLiteQuery()
        : Query(QUERY_FindDictionaryTerms,
                "FindDictionaryTermsSQLiteQuery",
                essential::DatabaseType::SQLite)
    {
    }

    nlohmann::json FindDictionaryTermsSQLiteQuery::call(
        nlohmann::json& request,
        api::InvalidateMethod& /*invalidate_method*/,
        plugins::core::models::OptionalError& /*optional_error*/)
    {
        nlohmann::json response;

        if (!request.contains("dictionary_map_id"))
        {
            throw std::invalid_argument("Mandatory key dictionary_map_id is missing");
        }
        if (!request.contains("title_part"))
        {
            throw std::invalid_argument("Mandatory key title_part is missing");
        }

        identification dictionary_map_id = request["dictionary_map_id"];
        bool any_map = dictionary_map_id == 0;

        std::string title_part = request["title_part"];

        bool include_aliases = false;
        if (request.contains("include_aliases"))
        {
            include_aliases = request["include_aliases"] == true;
        }

        int page_size = 20;
        int page_number = 1;

        bool random = title_part == "***";
        if (random)
        {
            include_aliases = false;
        }

        // ---------- SQL ----------

        static const std::string sql_random = R"(
SELECT
    id,
    title,
    disambiguation,
    '' AS alias_used,
    dictionary_map_id,
    0 AS relevance
FROM dictionary_term
WHERE (? = 1 OR dictionary_map_id = ?)
ORDER BY random()
LIMIT ?
)";

        static const std::string sql_relevance = R"(
SELECT
    t.id,
    t.title,
    t.disambiguation,
    COALESCE(a.alias, '') AS alias_used,
    t.dictionary_map_id,

    MAX(
        CASE
            WHEN t.title = ? THEN 100
            WHEN a.alias = ? THEN 90
            WHEN t.title LIKE ? THEN 70
            WHEN a.alias LIKE ? THEN 60
            WHEN t.title LIKE ? THEN 40
            WHEN a.alias LIKE ? THEN 30
            ELSE 0
        END
    ) AS relevance

FROM dictionary_term t
LEFT JOIN dictionary_term_alias a
       ON a.dictionary_term_id = t.id

WHERE
    (? = 1 OR t.dictionary_map_id = ?)
    AND (
        t.title LIKE ?
        OR (? = 1 AND a.alias LIKE ?)
    )

GROUP BY t.id
ORDER BY
    relevance DESC,
    t.title COLLATE NOCASE
LIMIT ? OFFSET ?
)";
        static const std::string select_all_maps = "select id, name from dictionary_map";

        const std::string& sql = random ? sql_random : sql_relevance;

        std::map<identification, std::string> map_names;

        if (any_map) try
        {
            SQLite::Database db(SQLITE_FILE_NAME, SQLite::OPEN_READONLY);
            db.exec("PRAGMA foreign_keys = ON;");
            db.exec("PRAGMA journal_mode = WAL;");

            SQLite::Statement query(db, select_all_maps);

            while (query.executeStep())
            {
                essential::debug << "Found map " + std::to_string(query.getColumn(0).getInt64())+ std::string(" ") + query.getColumn(1).getString() << essential::commit;
                identification id_ = query.getColumn(0).getInt64();
                auto name_ = query.getColumn(1).getString();
                map_names[id_] = name_;
            }
        }
        catch (const SQLite::Exception& e)
        {
            essential::err << e.what() << essential::commit;
        }

        try
        {
            SQLite::Database db(SQLITE_FILE_NAME, SQLite::OPEN_READONLY);
            db.exec("PRAGMA foreign_keys = ON;");
            db.exec("PRAGMA journal_mode = WAL;");

            SQLite::Statement query(db, sql);
            int i = 0;

            if (random)
            {
                query.bind(++i, any_map ? 1 : 0);
                query.bind(++i, dictionary_map_id);
                query.bind(++i, page_size);
            }
            else
            {
                std::string q_exact = title_part;
                std::string q_prefix = title_part + "%";
                std::string q_any = "%" + title_part + "%";

                // CASE scoring
                query.bind(++i, q_exact);   // title exact
                query.bind(++i, q_exact);   // alias exact
                query.bind(++i, q_prefix);  // title prefix
                query.bind(++i, q_prefix);  // alias prefix
                query.bind(++i, q_any);     // title substring
                query.bind(++i, q_any);     // alias substring

                // WHERE
                query.bind(++i, any_map ? 1 : 0);
                query.bind(++i, dictionary_map_id);
                query.bind(++i, q_any);
                query.bind(++i, include_aliases ? 1 : 0);
                query.bind(++i, q_any);

                // paging
                query.bind(++i, page_size);
                query.bind(++i, (page_number - 1) * page_size);
            }

            std::vector<nlohmann::json> results;

            while (query.executeStep())
            {
                nlohmann::json r;
                r["id"] = query.getColumn(0).getInt64();
                r["dictionary_term_id"] = r["id"];
                r["title"] = query.getColumn(1).getString();
                r["disambiguation"] = query.getColumn(2).getString();
                r["alias"] = query.getColumn(3).getString();
                r["dictionary_map_id"] = query.getColumn(4).getInt64();
                r["relevance"] = query.getColumn(5).getInt();
                if (any_map)
                {
                    identification dmi = query.getColumn(4).getInt64();
                    r["dictionary_map_name"] = (map_names.contains(dmi) ? map_names[dmi] : "?");
                }
                results.push_back(std::move(r));
            }

            response["results"] = std::move(results);
        }
        catch (const SQLite::Exception& e)
        {
            response["error"] = e.what();
            response["sql_failed"] = sql;
        }

        return response;
    }
}
