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

#include "../../../../../../../include/mind-net-plugin-dictionary/mindnet/plugins/dictionary/enums/TermStatus.hpp"
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

            auto split_csv = [](const std::string& s)
            {
                std::vector<std::string> out;
                std::stringstream ss(s);
                std::string item;
                while (std::getline(ss, item, ','))
                {
                    if (!item.empty())
                        out.push_back(item);
                }
                return out;
            };

            // text filters
            title_contains = q.value("title_contains", "");
            title_starts_with = q.value("title_starts_with", "");
            definition_contains = q.value("definition_contains", "Any");

            // enums / multi-selects serialized as comma-separated values
            statuses = split_csv(q.value("status", ""));
            pinned_only = q.value("pinned_only", false);

            difficulty_easy = q.value("difficulty_easy", true);
            difficulty_medium = q.value("difficulty_medium", true);
            difficulty_hard = q.value("difficulty_hard", true);

            importance_low = q.value("importance_low", true);
            importance_medium = q.value("importance_medium", true);
            importance_high = q.value("importance_high", true);

            tag_id = q.value("tag_id", 0);
            flag_title = q.value("flag_title", "");
            link_from_term_id = q.value("link_from_term_id", 0);
            link_to_term_id = q.value("link_to_term_id", 0);
            note_contains = q.value("note_contains", "");
            index_id = q.value("index_id", 0);
            source_id = q.value("source_id", 0);
            alias_alias = q.value("alias_alias", "");

            missing_items = split_csv(q.value("missing_items", ""));
            visited = q.value("visited", "Any");
            updated = q.value("updated", "Any");
            order = q.value("order", "Asc");
            if (order != "Asc" && order != "Desc") sort = "Asc";

            sort =q.value("sort", "None");
            std::vector sort_values = {"None", "Title", "Created at", "Updated at", "Status", "Difficulty", "Importance", "Random"};
            {
                bool sort_found = false;
                for (string e: sort_values)
                {
                    if (e == sort)
                    {
                        sort_found = true;
                        break;
                    }
                }
                if (!sort_found) sort = "None";
            }
        }

        // text filters
        string title_contains;
        string title_starts_with;
        string definition_contains;

        // enums / multi-selects serialized as comma-separated values
        vector<string> statuses;
        bool pinned_only = false;

        bool difficulty_easy = false;
        bool difficulty_medium = false;
        bool difficulty_hard = false;

        bool importance_low = false;
        bool importance_medium = false;
        bool importance_high = false;

        int tag_id = 0;
        string flag_title;
        int link_from_term_id = 0;
        int link_to_term_id = 0;
        string note_contains;
        int index_id = 0;
        int source_id = 0;
        string alias_alias;

        vector<string> missing_items;
        string visited;
        string updated;
        string sort;
        string order = "Asc";

        std::string to_json() const
        {
            using json = nlohmann::json;

            auto join_csv = [](const std::vector<std::string>& v)
            {
                std::ostringstream os;
                for (size_t i = 0; i < v.size(); ++i)
                {
                    if (i > 0) os << ",";
                    os << v[i];
                }
                return os.str();
            };

            json q;

            q["title_contains"] = title_contains;
            q["title_starts_with"] = title_starts_with;
            q["definition_contains"] = definition_contains;

            q["status"] = join_csv(statuses);
            q["pinned_only"] = pinned_only;

            q["difficulty_easy"] = difficulty_easy;
            q["difficulty_medium"] = difficulty_medium;
            q["difficulty_hard"] = difficulty_hard;

            q["importance_low"] = importance_low;
            q["importance_medium"] = importance_medium;
            q["importance_high"] = importance_high;

            q["tag_id"] = tag_id;
            q["flag_title"] = flag_title;
            q["link_from_term_id"] = link_from_term_id;
            q["link_to_term_id"] = link_to_term_id;
            q["note_contains"] = note_contains;
            q["index_id"] = index_id;
            q["source_id"] = source_id;
            q["alias_alias"] = alias_alias;

            q["missing_items"] = join_csv(missing_items);
            q["visited"] = visited;
            q["updated"] = updated;
            q["sort"] = sort;
            q["order"] = order;

            return q.dump(2);
        }
    };

    enum BindValueType
    {
        NUMBER, TEXT
    };

    struct BindValue
    {
    private:
        i64 number{0};
        std::string text;
        BindValueType type = TEXT;
    public:
        BindValue(i64 v) : number(v), type(NUMBER)
        {
        }
        BindValue(std::string v) : text(v), type(TEXT)
        {
        }
        const i64& get_number() const
        {
            if (type != NUMBER) throw std::runtime_error("This method cannot be called. The type is not number.");
            return number;
        }
        const std::string& get_text() const
        {
            if (type != TEXT) throw std::runtime_error("This method cannot be called. The type is not text.");
            return text;
        }
        const BindValueType& get_type() const
        {
            return type;
        }
    };

    static i64 now_ms()
    {
        using namespace std::chrono;
        return duration_cast<milliseconds>(
            system_clock::now().time_since_epoch()
        ).count();
    }

    FindDictionaryTermsViaAdvancedSearchSQLiteQuery::FindDictionaryTermsViaAdvancedSearchSQLiteQuery()
        : Query(QUERY_FindDictionaryTermsViaAdvancedSearch, "FindDictionaryTermsViaAdvancedSearchSQLiteQuery",
                essential::DatabaseType::SQLite)
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

        int page_size = request.value("page_size", 20);
        int page_number = request.value("page_number", 1);
        int offset = (page_number - 1) * page_size;

        std::string sql_start_page= "SELECT DISTINCT dt.id, dt.title, dt.disambiguation FROM dictionary_term dt ";
        std::string sql_start_count = "SELECT COUNT(*) FROM dictionary_term dt ";
        std::string sql_current_page = sql_start_page;
        std::vector<BindValue> binders;

        bool first_where = true;

        // joins
        if (q.tag_id > 0)
            sql_current_page += " JOIN dictionary_tag dtag ON dtag.dictionary_term_id = dt.id ";

        if (!q.flag_title.empty())
            sql_current_page += " JOIN dictionary_flag df ON df.dictionary_term_id = dt.id ";

        if (!q.alias_alias.empty())
            sql_current_page += " JOIN dictionary_term_alias da ON da.dictionary_term_id = dt.id ";

        if (q.link_from_term_id > 0)
            sql_current_page += " JOIN dictionary_link dl_from ON dl_from.to_dictionary_term_id = dt.id ";

        if (q.link_to_term_id > 0)
            sql_current_page += " JOIN dictionary_link dl_to ON dl_to.from_dictionary_term_id = dt.id ";

        if (!q.note_contains.empty())
            sql_current_page += " JOIN dictionary_note dn ON dn.dictionary_term_id = dt.id ";

        if (q.index_id > 0)
            sql_current_page += " JOIN dictionary_index di ON di.dictionary_term_id = dt.id ";

        if (q.source_id > 0)
            sql_current_page += " JOIN dictionary_source ds ON ds.dictionary_term_id = dt.id ";

        // pinned
        if (q.pinned_only)
        {
            sql_current_page +=
                " JOIN dictionary_pinned_term dpt ON dpt.dictionary_term_id = dt.id ";
            append_where(sql_current_page, first_where);
            sql_current_page += "dpt.user_id = ?";
            binders.push_back(user_id);
        }

        // mandatory
        append_where(sql_current_page, first_where);
        sql_current_page += "dt.dictionary_map_id = ?";
        binders.push_back(dictionary_map_id);

        // title filters
        if (!q.title_contains.empty())
        {
            append_where(sql_current_page, first_where);
            sql_current_page += "dt.title LIKE ?";
            binders.push_back("%" + q.title_contains + "%");
        }

        if (!q.title_starts_with.empty())
        {
            append_where(sql_current_page, first_where);
            sql_current_page += "dt.title LIKE ?";
            binders.push_back(q.title_starts_with + "%");
        }

        // definition
        if (!q.definition_contains.empty())
        {
            append_where(sql_current_page, first_where);
            sql_current_page += "dt.definition LIKE ?";
            binders.push_back("%" + q.definition_contains + "%");
        }

        // status
        if (!q.statuses.empty() && !(q.statuses.size() == 1 && q.statuses[0] == "Any"))
        {
            auto def = plugins::dictionary::enums::term_status_to_enum_definition();

            append_where(sql_current_page, first_where);
            sql_current_page += "dt.status IN (";
            for (size_t i = 0; i < q.statuses.size(); ++i)
            {
                if (i) sql_current_page += ",";
                sql_current_page += "?";
                auto status = q.statuses[i];
                auto status_int = def.get_value_as_int(status);
                if (status_int == -1) continue;
                binders.push_back(status_int);
            }
            sql_current_page += ")";
        }

        // difficulty
        if (!(q.difficulty_easy && q.difficulty_medium && q.difficulty_hard))
        {
            append_where(sql_current_page, first_where);
            sql_current_page += "dt.difficulty IN (";
            bool first = true;
            if (q.difficulty_easy)
            {
                sql_current_page += "1";
                first = false;
            }
            if (q.difficulty_medium)
            {
                if (!first) sql_current_page += ",";
                sql_current_page += "2";
                first = false;
            }
            if (q.difficulty_hard)
            {
                if (!first) sql_current_page += ",";
                sql_current_page += "3";
            }
            sql_current_page += ")";
        }

        // importance
        if (!(q.importance_low && q.importance_medium && q.importance_high))
        {
            append_where(sql_current_page, first_where);
            sql_current_page += "dt.importance IN (";
            bool first = true;
            if (q.importance_low)
            {
                sql_current_page += "1";
                first = false;
            }
            if (q.importance_medium)
            {
                if (!first) sql_current_page += ",";
                sql_current_page += "2";
                first = false;
            }
            if (q.importance_high)
            {
                if (!first) sql_current_page += ",";
                sql_current_page += "3";
            }
            sql_current_page += ")";
        }

        // tag
        if (q.tag_id > 0)
        {
            append_where(sql_current_page, first_where);
            sql_current_page += "dtag.dictionary_tag_type_id = ?";
            binders.push_back(q.tag_id);
        }

        // flag
        if (!q.flag_title.empty())
        {
            append_where(sql_current_page, first_where);
            sql_current_page += "df.title = ? AND df.user_id = ?";
            binders.push_back(q.flag_title);
            binders.push_back(user_id);
        }

        // alias
        if (!q.alias_alias.empty())
        {
            append_where(sql_current_page, first_where);
            sql_current_page += "da.alias LIKE ?";
            binders.push_back("%" + q.alias_alias + "%");
        }

        // note
        if (!q.note_contains.empty())
        {
            append_where(sql_current_page, first_where);
            sql_current_page += "dn.content LIKE ?";
            binders.push_back("%" + q.note_contains + "%");
        }

        // index
        if (q.index_id > 0)
        {
            append_where(sql_current_page, first_where);
            sql_current_page += "di.dictionary_index_type_id = ?";
            binders.push_back(q.index_id);
        }

        // source
        if (q.source_id > 0)
        {
            append_where(sql_current_page, first_where);
            sql_current_page += "ds.dictionary_source_type_id = ?";
            binders.push_back(q.source_id);
        }

        // missing items
        if (!q.missing_items.empty())
        {
            // common NOT EXISTS patterns
            static const std::unordered_map<std::string, std::string> missing_tables = {
                {"notes",    "dictionary_note dn"},
                {"aliases",  "dictionary_term_alias da"},
                {"tags",     "dictionary_tag dtag"},
                {"flags",    "dictionary_flag df"},
                {"sources",  "dictionary_source ds"},
                {"indexes",  "dictionary_index di"}
            };

            for (const auto& item : q.missing_items)
            {
                std::string condition;

                // --- special cases ---
                if (item == "definition")
                {
                    condition = "(dt.definition IS NULL OR dt.definition = '')";
                }
                else if (item == "links")
                {
                    condition =
                        "NOT EXISTS (SELECT 1 FROM dictionary_link dl "
                        "WHERE dl.from_dictionary_term_id = dt.id "
                        "   OR dl.to_dictionary_term_id = dt.id)";
                }
                // --- generic NOT EXISTS ---
                else if (auto it = missing_tables.find(item); it != missing_tables.end())
                {
                    condition =
                        "NOT EXISTS (SELECT 1 FROM " + it->second +
                        " WHERE " + it->second.substr(it->second.find(' ') + 1) +
                        ".dictionary_term_id = dt.id)";
                }

                // append only if we really added a condition
                if (!condition.empty())
                {
                    append_where(sql_current_page, first_where);
                    sql_current_page += condition;
                }
            }
        }
        static constexpr i64 MS_PER_HOUR = 60LL * 60 * 1000;
        // visited
        if (q.visited != "Any")
        {
            if (q.visited == "Never")
            {
                append_where(sql_current_page, first_where);
                sql_current_page +=
                    "NOT EXISTS (SELECT 1 FROM dictionary_term_visit dtv "
                    "WHERE dtv.dictionary_term_id = dt.id "
                    "AND dtv.user_id = ?)";
                binders.push_back(user_id);
            }
            else
            {
                bool negated = q.visited.rfind("Not ", 0) == 0;
                std::string base = negated ? q.visited.substr(4) : q.visited;

                i64 threshold_ms = 0;
                i64 now = now_ms();

                if (base == "Last hour")
                    threshold_ms = now - 1LL * MS_PER_HOUR;
                else if (base == "Last 3 hours")
                    threshold_ms = now - 3LL * MS_PER_HOUR;
                else if (base == "Today")
                    threshold_ms = now - 24LL * MS_PER_HOUR;
                else if (base == "Last week")
                    threshold_ms = now - 7LL * 24 * MS_PER_HOUR;
                else if (base == "Last month")
                    threshold_ms = now - 30LL * 24 * MS_PER_HOUR;
                else if (base == "Last year")
                    threshold_ms = now - 365LL * 24 * MS_PER_HOUR;
                else if (base == "Last 10 years")
                    threshold_ms = now - 3650LL * 24 * MS_PER_HOUR;

                if (threshold_ms > 0)
                {
                    append_where(sql_current_page, first_where);

                    if (!negated)
                    {
                        sql_current_page +=
                            "EXISTS (SELECT 1 FROM dictionary_term_visit dtv "
                            "WHERE dtv.dictionary_term_id = dt.id "
                            "AND dtv.user_id = ? "
                            "AND dtv.created_at >= ?)";
                    }
                    else
                    {
                        sql_current_page +=
                            "NOT EXISTS (SELECT 1 FROM dictionary_term_visit dtv "
                            "WHERE dtv.dictionary_term_id = dt.id "
                            "AND dtv.user_id = ? "
                            "AND dtv.created_at >= ?)";
                    }

                    binders.push_back(user_id);
                    binders.push_back(threshold_ms);
                }
            }
        }


        // updated
        if (q.updated != "Any")
        {
            if (q.updated == "Never")
            {
                append_where(sql_current_page, first_where);
                sql_current_page +=
                    "(dt.updated_at IS NULL OR dt.updated_at = dt.created_at)";
            }
            else
            {
                bool negated = q.updated.rfind("Not ", 0) == 0;
                std::string base = negated ? q.updated.substr(4) : q.updated;

                i64 threshold_ms = 0;
                i64 now = now_ms();

                if (base == "Last hour")
                    threshold_ms = now - 1LL * MS_PER_HOUR;
                else if (base == "Last 3 hours")
                    threshold_ms = now - 3LL * MS_PER_HOUR;
                else if (base == "Today")
                    threshold_ms = now - 24LL * MS_PER_HOUR;
                else if (base == "Last week")
                    threshold_ms = now - 7LL * 24 * MS_PER_HOUR;
                else if (base == "Last month")
                    threshold_ms = now - 30LL * 24 * MS_PER_HOUR;
                else if (base == "Last year")
                    threshold_ms = now - 365LL * 24 * MS_PER_HOUR;
                else if (base == "Last 10 years")
                    threshold_ms = now - 3650LL * 24 * MS_PER_HOUR;

                if (threshold_ms > 0)
                {
                    append_where(sql_current_page, first_where);

                    if (!negated)
                    {
                        sql_current_page +=
                            "dt.updated_at >= ?";
                    }
                    else
                    {
                        sql_current_page +=
                            "(dt.updated_at < ? OR dt.updated_at IS NULL)";
                    }

                    binders.push_back(threshold_ms);
                }
            }
        }




        std::string sql_sort = "";
        if (q.sort == "Title") sql_sort = "dt.title";
        if (q.sort == "Created at") sql_sort = "dt.created_at";
        if (q.sort == "Updated at") sql_sort = "dt.updated_at";
        if (q.sort == "Status") sql_sort = "dt.status";
        if (q.sort == "Difficulty") sql_sort = "dt.difficulty";
        if (q.sort == "Importance") sql_sort = "dt.importance";
        if (q.sort == "Random") sql_sort = "random()";
        if (!sql_sort.empty())
        {
            sql_sort = " ORDER BY " + sql_sort + " " + q.order + " ";
        }

        static std::string sql_limit_offset = " LIMIT ? OFFSET ?";
        
        std::string sql_count;

        try
        {
            SQLite::Database db(SQLITE_FILE_NAME, SQLite::OPEN_READONLY);

            {
                sql_current_page += sql_sort;
                sql_current_page += sql_limit_offset;
                SQLite::Statement stmt(db, sql_current_page);

                int i = 0;
                for (const BindValue& b: binders)
                {
                    switch (b.get_type())
                    {
                    case TEXT: stmt.bind(++i, b.get_text()); break;
                    case NUMBER: stmt.bind(++i, b.get_number()); break;
                    default: throw std::runtime_error(std::string("Unknown type: ") + std::to_string(b.get_type()));
                    }
                }

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
            sql_count = sql_current_page;
            {
                sql_count.erase(0, sql_start_page.size());
                sql_count = sql_start_count + sql_count;
                sql_count.erase(sql_count.size() - sql_limit_offset.size());

                SQLite::Statement stmt(db, sql_count);

                int i = 0;
                for (const BindValue& b: binders)
                {
                    switch (b.get_type())
                    {
                    case TEXT: stmt.bind(++i, b.get_text()); break;
                    case NUMBER: stmt.bind(++i, b.get_number()); break;
                    default: throw std::runtime_error(std::string("Unknown type: ") + std::to_string(b.get_type()));
                    }
                }

                nlohmann::json arr = nlohmann::json::array();
                if (stmt.executeStep())
                {
                    response["total_items"] = stmt.getColumn(0).getInt();
                }
            }

        }
        catch (const SQLite::Exception& e)
        {
            response["error"] = e.what();
            response["sql"] = sql_current_page;
            response["sql2"] = sql_count;
        }

        return response;
    }
}
