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

#include "../../../../../../../include/mind-net-db-sqlite/mindnet/db/sqlite/queries/dictionary/FindDictionaryTermsViaAdvancedSearchSQLiteQuery.hpp"
#include <SQLiteCpp/SQLiteCpp.h>
#include "mindnet/db/sqlite/SqliteFileName.hpp"
#include "mindnet/essential/DatabaseType.hpp"
#include "mindnet/essential/Global.hpp"

namespace mindnet::db::sqlite::queries::dictionary
{
    using std::string;
    using std::vector;
    struct SearchQuery
    {
        SearchQuery(const std::string& json_string)
        {
            using json = nlohmann::json;
            json q = json::parse(json_string);

            auto split_csv = [](const std::string& s) {
                std::vector<std::string> out;
                std::stringstream ss(s);
                std::string item;
                while (std::getline(ss, item, ',')) {
                    if (!item.empty())
                        out.push_back(item);
                }
                return out;
            };

            // text filters
            title_contains      = q.value("title_contains", "");
            title_starts_with   = q.value("title_starts_with", "");
            definition_contains = q.value("definition_contains", "Any");

            // enums / multi-selects serialized as comma-separated values
            statuses    = split_csv(q.value("status", ""));
            pinned_only = q.value("pinned_only", false);

            difficulty_easy   = q.value("difficulty_easy", true);
            difficulty_medium = q.value("difficulty_medium", true);
            difficulty_hard   = q.value("difficulty_hard", true);

            importance_low    = q.value("importance_low", true);
            importance_medium = q.value("importance_medium", true);
            importance_high   = q.value("importance_high", true);

            tag_id            = q.value("tag_id", 0);
            flag_title        = q.value("flag_title", "");
            link_from_term_id = q.value("link_from_term_id", 0);
            link_to_term_id   = q.value("link_to_term_id", 0);
            note_contains     = q.value("note_contains", "");
            index_id          = q.value("index_id", 0);
            source_id         = q.value("source_id", 0);
            alias_alias       = q.value("alias_alias", "");

            has_items = split_csv(q.value("has_items", ""));
            visited   = q.value("visited", "Any");
            updated   = q.value("updated", "Any");
        }

        // text filters
        string title_contains;
        string title_starts_with;
        string definition_contains;

        // enums / multi-selects serialized as comma-separated values
        vector<string> statuses;
        bool pinned_only = false;

        bool difficulty_easy   = false;
        bool difficulty_medium = false;
        bool difficulty_hard   = false;

        bool importance_low    = false;
        bool importance_medium = false;
        bool importance_high   = false;

        int tag_id            = 0;
        string flag_title;
        int link_from_term_id = 0;
        int link_to_term_id   = 0;
        string note_contains;
        int index_id          = 0;
        int source_id         = 0;
        string alias_alias;

        vector<string> has_items;
        string visited;
        string updated;

        std::string to_json() const
        {
            using json = nlohmann::json;

            auto join_csv = [](const std::vector<std::string>& v) {
                std::ostringstream os;
                for (size_t i = 0; i < v.size(); ++i) {
                    if (i > 0) os << ",";
                    os << v[i];
                }
                return os.str();
            };

            json q;

            q["title_contains"]      = title_contains;
            q["title_starts_with"]   = title_starts_with;
            q["definition_contains"] = definition_contains;

            q["status"]      = join_csv(statuses);
            q["pinned_only"] = pinned_only;

            q["difficulty_easy"]   = difficulty_easy;
            q["difficulty_medium"] = difficulty_medium;
            q["difficulty_hard"]   = difficulty_hard;

            q["importance_low"]    = importance_low;
            q["importance_medium"] = importance_medium;
            q["importance_high"]   = importance_high;

            q["tag_id"]            = tag_id;
            q["flag_title"]        = flag_title;
            q["link_from_term_id"] = link_from_term_id;
            q["link_to_term_id"]   = link_to_term_id;
            q["note_contains"]     = note_contains;
            q["index_id"]          = index_id;
            q["source_id"]         = source_id;
            q["alias_alias"]       = alias_alias;

            q["has_items"] = join_csv(has_items);
            q["visited"]   = visited;
            q["updated"]   = updated;

            return q.dump(); // případně dump(2) pro pretty print
        }

    };
    enum BindValueType
    {
        NUMBER, TEXT
    };
    struct BindValue
    {
        i64 number{0};
        std::string text;
        BindValueType type = TEXT;
    };

    FindDictionaryTermsViaAdvancedSearchSQLiteQuery::FindDictionaryTermsViaAdvancedSearchSQLiteQuery()
        : Query(QUERY_FindDictionaryTermsViaAdvancedSearch,"FindDictionaryTermsViaAdvancedSearchSQLiteQuery", essential::DatabaseType::SQLite)
    {
    }

    static void append_where(std::string& sql, bool& first)
    {
        if (first)
        {
            sql += " WHERE ";
            first = false;
        }
        else
        {
            sql += " AND ";
        }
    }

  nlohmann::json FindDictionaryTermsViaAdvancedSearchSQLiteQuery::call(
        nlohmann::json& request,
        api::InvalidateMethod& invalidate_method)
{
    nlohmann::json response;

    if (!request.contains("user_id"))
        throw std::invalid_argument("Mandatory key user_id is missing");
    identification user_id = request["user_id"];

    if (!request.contains("dictionary_map_id"))
        throw std::invalid_argument("Mandatory key dictionary_map_id is missing");
    identification dictionary_map_id = request["dictionary_map_id"];

    if (!request.contains("query_json"))
        throw std::invalid_argument("Mandatory key query_json is missing");

    SearchQuery q(request["query_json"]);
    essential::debug << q.to_json() << essential::commit;

    int page_size   = request.value("page_size", 20);
    int page_number = request.value("page_number", 1);
    int offset      = (page_number - 1) * page_size;

    std::string sql =
        "SELECT DISTINCT dt.id, dt.title, dt.disambiguation "
        "FROM dictionary_term dt ";

    bool first_where = true;

    // joins
    if (q.tag_id > 0)
        sql += " JOIN dictionary_tag dtag ON dtag.dictionary_term_id = dt.id ";

    if (!q.flag_title.empty())
        sql += " JOIN dictionary_flag df ON df.dictionary_term_id = dt.id ";

    if (!q.alias_alias.empty())
        sql += " JOIN dictionary_term_alias da ON da.dictionary_term_id = dt.id ";

    if (q.link_from_term_id > 0)
        sql += " JOIN dictionary_link dl_from ON dl_from.to_dictionary_term_id = dt.id ";

    if (q.link_to_term_id > 0)
        sql += " JOIN dictionary_link dl_to ON dl_to.from_dictionary_term_id = dt.id ";

    if (!q.note_contains.empty())
        sql += " JOIN dictionary_note dn ON dn.dictionary_term_id = dt.id ";

    if (q.index_id > 0)
        sql += " JOIN dictionary_index di ON di.dictionary_term_id = dt.id ";

    if (q.source_id > 0)
        sql += " JOIN dictionary_source ds ON ds.dictionary_term_id = dt.id ";

    // mandatory
    append_where(sql, first_where);
    sql += "dt.dictionary_map_id = ?";

    // title filters
    if (!q.title_contains.empty())
    {
        append_where(sql, first_where);
        sql += "dt.title LIKE ?";
    }

    if (!q.title_starts_with.empty())
    {
        append_where(sql, first_where);
        sql += "dt.title LIKE ?";
    }

    // definition
    if (!q.definition_contains.empty())
    {
        append_where(sql, first_where);
        sql += "dt.definition LIKE ?";
    }

    // status
    if (!q.statuses.empty() && !(q.statuses.size() == 1 && q.statuses[0] == "Any"))
    {
        append_where(sql, first_where);
        sql += "dt.status IN (";
        for (size_t i = 0; i < q.statuses.size(); ++i)
        {
            if (i) sql += ",";
            sql += "?";
        }
        sql += ")";
    }

    // difficulty
    if (!(q.difficulty_easy && q.difficulty_medium && q.difficulty_hard))
    {
        append_where(sql, first_where);
        sql += "dt.difficulty IN (";
        bool first = true;
        if (q.difficulty_easy)   { sql += "0"; first = false; }
        if (q.difficulty_medium) { if (!first) sql += ","; sql += "1"; first = false; }
        if (q.difficulty_hard)   { if (!first) sql += ","; sql += "2"; }
        sql += ")";
    }

    // importance
    if (!(q.importance_low && q.importance_medium && q.importance_high))
    {
        append_where(sql, first_where);
        sql += "dt.importance IN (";
        bool first = true;
        if (q.importance_low)    { sql += "0"; first = false; }
        if (q.importance_medium) { if (!first) sql += ","; sql += "1"; first = false; }
        if (q.importance_high)   { if (!first) sql += ","; sql += "2"; }
        sql += ")";
    }

    // tag
    if (q.tag_id > 0)
    {
        append_where(sql, first_where);
        sql += "dtag.dictionary_tag_type_id = ?";
    }

    // flag
    if (!q.flag_title.empty())
    {
        append_where(sql, first_where);
        sql += "df.title = ? AND df.user_id = ?";
    }

    // pinned
    if (q.pinned_only)
    {
        sql +=
            " JOIN dictionary_pinned_term dpt ON dpt.dictionary_term_id = dt.id ";
        append_where(sql, first_where);
        sql += "dpt.user_id = ?";
    }

    // alias
    if (!q.alias_alias.empty())
    {
        append_where(sql, first_where);
        sql += "da.alias LIKE ?";
    }

    // note
    if (!q.note_contains.empty())
    {
        append_where(sql, first_where);
        sql += "dn.content LIKE ?";
    }

    // index
    if (q.index_id > 0)
    {
        append_where(sql, first_where);
        sql += "di.dictionary_index_type_id = ?";
    }

    // source
    if (q.source_id > 0)
    {
        append_where(sql, first_where);
        sql += "ds.dictionary_source_type_id = ?";
    }

    sql += " ORDER BY dt.title ASC LIMIT ? OFFSET ?";

    try
    {
        SQLite::Database db(SQLITE_FILE_NAME, SQLite::OPEN_READONLY);
        SQLite::Statement stmt(db, sql);

        int i = 0;
        stmt.bind(++i, dictionary_map_id);

        if (!q.title_contains.empty())
            stmt.bind(++i, "%" + q.title_contains + "%");

        if (!q.title_starts_with.empty())
            stmt.bind(++i, q.title_starts_with + "%");

        if (!q.definition_contains.empty())
            stmt.bind(++i, "%" + q.definition_contains + "%");

        for (const auto& s : q.statuses)
        {
            if (s == "Any") continue;
            stmt.bind(++i, s);
        }

        if (q.tag_id > 0)
            stmt.bind(++i, q.tag_id);

        if (!q.flag_title.empty())
        {
            stmt.bind(++i, q.flag_title);
            stmt.bind(++i, user_id);
        }

        if (q.pinned_only)
            stmt.bind(++i, user_id);

        if (!q.alias_alias.empty())
            stmt.bind(++i, "%" + q.alias_alias + "%");

        if (!q.note_contains.empty())
            stmt.bind(++i, "%" + q.note_contains + "%");

        if (q.index_id > 0)
            stmt.bind(++i, q.index_id);

        if (q.source_id > 0)
            stmt.bind(++i, q.source_id);

        stmt.bind(++i, page_size);
        stmt.bind(++i, offset);

        nlohmann::json arr = nlohmann::json::array();
        while (stmt.executeStep())
        {
            arr.push_back({
                {"id", stmt.getColumn(0).getInt()},
                {"title", stmt.getColumn(1).getString()},
                {"disambiguation", stmt.getColumn(2).getString()}
            });
        }

        response["results"] = arr;
    }
    catch (const SQLite::Exception& e)
    {
        response["error"] = e.what();
        response["sql"]   = sql;
    }

    return response;
}

}
