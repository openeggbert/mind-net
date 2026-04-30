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

#include "../../../../../../../include/hive-db-sqlite/hive/db/sqlite/queries/dictionary/FindDictionaryTermsViaAdvancedSearchSQLiteQuery.hpp"
#include <SQLiteCpp/SQLiteCpp.h>

#include "../../../../../../../include/hive-plugin-dictionary/hive/plugins/dictionary/enums/TermStatus.hpp"
#include "hive/db/sqlite/SqliteFileName.hpp"
#include "hive/essential/DatabaseType.hpp"
#include "hive/essential/Global.hpp"
#include "hive/plugins/core/ErrorBuilder.hpp"
#include "hive/plugins/core/models/Error.hpp"
#include "hive/essential/EnumUtils.hpp"

#include <cctype>

namespace hive::db::sqlite::queries::dictionary
{
    static const std::string CPP_NAMESPACE = "hive::db::sqlite::queries::dictionary";
    using std::string;
    using std::vector;

    void capitalize_first(std::string& s)
    {
        if (!s.empty())
        {
            s[0] = static_cast<char>(std::toupper(
                static_cast<unsigned char>(s[0])
            ));
        }
    }

#define TERM_STATUS_LIST(X, ENUM_NAME) \
X(Any,        -1, ENUM_NAME)       \
X(NotDefined,  0, ENUM_NAME)       \
\
X(Stub,        1, ENUM_NAME)       \
X(Draft,       2, ENUM_NAME)       \
X(Incomplete,  3, ENUM_NAME)       \
X(Verified,    4, ENUM_NAME)       \
X(Deprecated,  5, ENUM_NAME)       \
X(Deleted,     6, ENUM_NAME)

    DECLARE_ENUM(TermStatus, term_status, TERM_STATUS_LIST)
////

    #define UNDERSTANDING_LEVEL_LIST(X, ENUM_NAME) \
X(Any,          -1, ENUM_NAME)             \
\
X(Unknown,       0, ENUM_NAME)             \
X(Recognized,    1, ENUM_NAME)             \
X(Understood,    2, ENUM_NAME)             \
X(Applied,       3, ENUM_NAME)             \
X(Internalized,  4, ENUM_NAME)

DECLARE_ENUM(UnderstandingLevel, understanding_level, UNDERSTANDING_LEVEL_LIST)

////
#define IMPORTANCE_LIST(X, ENUM_NAME) \
X(Low,    1, ENUM_NAME)           \
X(Medium, 2, ENUM_NAME)           \
X(High,   3, ENUM_NAME)

    DECLARE_ENUM(Importance, importance, IMPORTANCE_LIST)

#define DIFFICULTY_LIST(X, ENUM_NAME) \
X(Easy,   1, ENUM_NAME)           \
X(Medium, 2, ENUM_NAME)           \
X(Hard,   3, ENUM_NAME)

    DECLARE_ENUM(Difficulty, difficulty, DIFFICULTY_LIST)

#define DICTIONARY_ITEM_LIST(X, ENUM_NAME) \
X(Definition, 1, ENUM_NAME)            \
X(Tags,       2, ENUM_NAME)            \
X(Flags,      3, ENUM_NAME)            \
X(Links,      4, ENUM_NAME)            \
X(Notes,      5, ENUM_NAME)            \
X(Indexes,    6, ENUM_NAME)            \
X(Sources,    7, ENUM_NAME)            \
X(Aliases,    8, ENUM_NAME)

    DECLARE_ENUM(DictionaryItem, dictionary_item, DICTIONARY_ITEM_LIST)

#define TIME_RANGE_LIST(X, ENUM_NAME)        \
X(Any,            0,   ENUM_NAME)       \
\
X(LastHour,       1,   ENUM_NAME)       \
X(Last3Hours,     2,   ENUM_NAME)       \
X(Last24Hours,          3,   ENUM_NAME)       \
X(LastWeek,       4,   ENUM_NAME)       \
X(LastMonth,      5,   ENUM_NAME)       \
X(LastYear,       6,   ENUM_NAME)       \
X(Last10Years,    7,   ENUM_NAME)       \
\
X(NotLastHour,    21,  ENUM_NAME)       \
X(NotLast3Hours,  22,  ENUM_NAME)       \
X(NotLast24Hours,       23,  ENUM_NAME)       \
X(NotLastWeek,    24,  ENUM_NAME)       \
X(NotLastMonth,   25,  ENUM_NAME)       \
X(NotLastYear,    26,  ENUM_NAME)       \
X(NotLast10Years, 27,  ENUM_NAME)       \
\
X(Never,          100, ENUM_NAME)

    DECLARE_ENUM(TimeRange, time_range, TIME_RANGE_LIST)

    TimeRange make_absolute_time_range(TimeRange tr)
    {
        switch (tr)
        {
        case TimeRange::NotLastHour: return TimeRange::LastHour;
        case TimeRange::NotLast3Hours: return TimeRange::Last3Hours;
        case TimeRange::NotLast24Hours: return TimeRange::Last24Hours;
        case TimeRange::NotLastWeek: return TimeRange::LastWeek;
        case TimeRange::NotLastMonth: return TimeRange::LastMonth;
        case TimeRange::NotLastYear: return TimeRange::LastYear;
        case TimeRange::NotLast10Years: return TimeRange::Last10Years;

        default:
            return tr; // Any, Never, Last*
        }
    }

    static constexpr i64 MS_PER_HOUR = 60LL * 60 * 1000;

    i64 threshold_from_time_range(TimeRange tr, i64 now)
    {
        tr = make_absolute_time_range(tr);

        switch (tr)
        {
        case TimeRange::LastHour:
            return now - 1LL * MS_PER_HOUR;

        case TimeRange::Last3Hours:
            return now - 3LL * MS_PER_HOUR;

        case TimeRange::Last24Hours:
            return now - 24LL * MS_PER_HOUR;

        case TimeRange::LastWeek:
            return now - 7LL * 24 * MS_PER_HOUR;

        case TimeRange::LastMonth:
            return now - 30LL * 24 * MS_PER_HOUR;

        case TimeRange::LastYear:
            return now - 365LL * 24 * MS_PER_HOUR;

        case TimeRange::Last10Years:
            return now - 3650LL * 24 * MS_PER_HOUR;

        default:
            return 0; // Any, Never
        }
    }

    bool is_negative_time_range(TimeRange tr)
    {
        switch (tr)
        {
        case TimeRange::NotLastHour:
        case TimeRange::NotLast3Hours:
        case TimeRange::NotLast24Hours:
        case TimeRange::NotLastWeek:
        case TimeRange::NotLastMonth:
        case TimeRange::NotLastYear:
        case TimeRange::NotLast10Years:
            return true;
        default:
            return false;
        }
    }


#define REPETITION_MODE_LIST(X, ENUM_NAME) \
X(Due,     1, ENUM_NAME)               \
X(NotDue,  2, ENUM_NAME)               \
X(Never,   3, ENUM_NAME)

    DECLARE_ENUM(RepetitionMode, repetition_mode, REPETITION_MODE_LIST)


#define IS_FOR_REPETITION_MODE_LIST(X, ENUM_NAME) \
X(Any,     1, ENUM_NAME)               \
X(ForRepetition,  2, ENUM_NAME)               \
X(NotForRepetition,   3, ENUM_NAME)
    DECLARE_ENUM(IsForRepetitionMode, is_for_repetition_mode, IS_FOR_REPETITION_MODE_LIST)


#define SORT_LIST(X, ENUM_NAME)        \
X(None,        0,  ENUM_NAME)     \
X(Title,       1,  ENUM_NAME)     \
X(CreatedAt,   2,  ENUM_NAME)     \
X(UpdatedAt,   3,  ENUM_NAME)     \
X(Status,      11, ENUM_NAME)     \
X(Difficulty,  12, ENUM_NAME)     \
X(Importance,  13, ENUM_NAME)     \
X(NextReview,  14, ENUM_NAME)     \
X(Random,      30, ENUM_NAME)

    DECLARE_ENUM(Sort, sort, SORT_LIST)

#define ORDER_LIST(X, ENUM_NAME) \
X(None, 0, ENUM_NAME)        \
X(Asc,  1, ENUM_NAME)        \
X(Desc, 2, ENUM_NAME)

    DECLARE_ENUM(Order, order, ORDER_LIST)

    struct SearchModel
    {
        identification map_id = 0;

        // text filters
        string title_contains;
        string title_starts_with;
        string definition_contains;

        // enums / multi-selects serialized as comma-separated values
        vector<TermStatus> statuses;
        vector<UnderstandingLevel> understandings;
        bool pinned_only = false;

        bool difficulty_easy = false;
        bool difficulty_medium = false;
        bool difficulty_hard = false;

        bool importance_low = false;
        bool importance_medium = false;
        bool importance_high = false;
        IsForRepetitionMode is_for_repetition = IsForRepetitionMode::Any;
        bool root_only = false;

        int tag_id = 0;
        string flag_title;
        int link_from_term_id = 0;
        int link_to_term_id = 0;
        string note_contains;
        int index_id = 0;
        int source_id = 0;

        string alias_contains;
        string alias_alias;

        vector<DictionaryItem> has_items;
        vector<DictionaryItem> missing_items;
        TimeRange created = TimeRange::Any;
        TimeRange updated = TimeRange::Any;
        TimeRange visited = TimeRange::Any;
        TimeRange reviewed = TimeRange::Any;

        bool repetition_due;
        bool repetition_not_due;
        bool repetition_never;

        Sort sort = Sort::None;
        Order order = Order::None;

        SearchModel(const std::string& json_string, identification default_map_id)
        {
            using json = nlohmann::json;
            json q = json::parse(json_string);

            map_id = q.value("map_id", 0);
            if (!q.contains("map_id"))
            {
                map_id = default_map_id;
            }
            // text filters
            title_contains = q.value("title_contains", "");
            title_starts_with = q.value("title_starts_with", "");
            definition_contains = q.value("definition_contains", "Any");

            // enums / multi-selects serialized as comma-separated values
            for (auto& e : q.at("statuses").get<std::vector<int>>())
            {
                if (e == -1) continue; //Any
                statuses.push_back(int_to_term_status(e));
            }
            for (auto& e : q.at("understandings").get<std::vector<int>>())
            {
                if (e == -1) continue; //Any
                understandings.push_back(int_to_understanding_level(e));
            }
            pinned_only = q.value("pinned_only", false);

            difficulty_easy = q.value("difficulty_easy", true);
            difficulty_medium = q.value("difficulty_medium", true);
            difficulty_hard = q.value("difficulty_hard", true);

            importance_low = q.value("importance_low", true);
            importance_medium = q.value("importance_medium", true);
            importance_high = q.value("importance_high", true);

            int is_for_repetition_int = q.value("is_for_repetition", static_cast<int>(IsForRepetitionMode::Any));
            is_for_repetition = int_to_is_for_repetition_mode(is_for_repetition_int);
            root_only = q.value("root_only", false);

            tag_id = q.value("tag_id", 0);
            flag_title = q.value("flag_title", "");
            link_from_term_id = q.value("link_from_term_id", 0);
            link_to_term_id = q.value("link_to_term_id", 0);
            note_contains = q.value("note_contains", "");
            index_id = q.value("index_id", 0);
            source_id = q.value("source_id", 0);

            alias_contains = q.value("alias_contains", "");
            alias_alias = q.value("alias_alias", "");

            for (auto& e : q.at("has_items").get<std::vector<int>>())
                has_items.push_back(int_to_dictionary_item(e));
            for (auto& e : q.at("missing_items").get<std::vector<int>>())
                missing_items.push_back(int_to_dictionary_item(e));

            created = int_to_time_range(q.at("created").get<int>());
            updated = int_to_time_range(q.at("updated").get<int>());
            visited = int_to_time_range(q.at("visited").get<int>());
            reviewed = int_to_time_range(q.at("reviewed").get<int>());

            repetition_due = q.value("repetition_due", true);
            repetition_not_due = q.value("repetition_not_due", true);
            repetition_never = q.value("repetition_never", true);

            if (!repetition_due && !repetition_not_due && !repetition_never)
            {
                repetition_due = true;
                repetition_never = true;
            }
            order = int_to_order(q.at("order").get<int>());
            sort = int_to_sort(q.at("sort").get<int>());
        }

        [[nodiscard]] std::string to_json() const
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

            q["map_id"] = map_id;
            q["title_contains"] = title_contains;
            q["title_starts_with"] = title_starts_with;
            q["definition_contains"] = definition_contains;

            q["statuses"] = statuses;
            q["understandings"] = understandings;
            q["pinned_only"] = pinned_only;

            q["difficulty_easy"] = difficulty_easy;
            q["difficulty_medium"] = difficulty_medium;
            q["difficulty_hard"] = difficulty_hard;

            q["importance_low"] = importance_low;
            q["importance_medium"] = importance_medium;
            q["importance_high"] = importance_high;
            q["is_for_repetition"] = is_for_repetition;
            q["root_only"] = root_only;

            q["tag_id"] = tag_id;
            q["flag_title"] = flag_title;
            q["link_from_term_id"] = link_from_term_id;
            q["link_to_term_id"] = link_to_term_id;
            q["note_contains"] = note_contains;
            q["index_id"] = index_id;
            q["source_id"] = source_id;
            q["alias_contains"] = alias_contains;
            q["alias_alias"] = alias_alias;

            q["has_items"] = has_items;
            q["missing_items"] = missing_items;
            q["created"] = created;
            q["updated"] = updated;
            q["visited"] = visited;
            q["reviewed"] = reviewed;

            q["repetition_due"] = repetition_due;
            q["repetition_not_due"] = repetition_not_due;
            q["repetition_never"] = repetition_never;

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

        BindValue(const std::string& v) : text(v), type(TEXT)
        {
        }

        [[nodiscard]] const i64& get_number() const
        {
            if (type != NUMBER) throw std::runtime_error("This method cannot be called. The type is not number.");
            return number;
        }

        [[nodiscard]] const std::string& get_text() const
        {
            if (type != TEXT) throw std::runtime_error("This method cannot be called. The type is not text.");
            return text;
        }

        [[nodiscard]] const BindValueType& get_type() const
        {
            return type;
        }
    };

    static i64 get_now_ms()
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

    static const std::string CPP_CLASS = "FindDictionaryTermsViaAdvancedSearchSQLiteQuery";

    nlohmann::json FindDictionaryTermsViaAdvancedSearchSQLiteQuery::call(
        nlohmann::json& request,
        api::InvalidateMethod& invalidate_method, plugins::core::models::OptionalError& optional_error)
    {
        static const std::string CPP_SYMBOL =
            "call(nlohmann::json& request, api::InvalidateMethod& invalidate_method, plugins::core::models::OptionalError& optional_error)";
        nlohmann::json response;

        if (!request.contains("user_id"))
            throw std::invalid_argument("Mandatory key user_id is missing");
        identification user_id = request["user_id"];

        if (!request.contains("dictionary_map_id"))
            throw std::invalid_argument("Mandatory key dictionary_map_id is missing");
        identification dictionary_map_id = request["dictionary_map_id"];
        bool any_map = dictionary_map_id == 0;

        if (!request.contains("query_json"))
            throw std::invalid_argument("Mandatory key query_json is missing");

        SearchModel q(request["query_json"], dictionary_map_id);
        essential::debug << q.to_json() << essential::commit;
        if (dictionary_map_id != q.map_id)
        {
            throw std::invalid_argument("dictionary_map_id mismatch");
        }

        int page_size = request.value("page_size", 20);
        int page_number = request.value("page_number", 1);
        int offset = (page_number - 1) * page_size;
        i64 now_ms = get_now_ms();

        std::string sql_start_page =
            //any_map ?
        "SELECT DISTINCT dt.id, dt.title, dt.disambiguation, dt.dictionary_map_id, dt.created_at, dt.updated_at, dt.status, dt.importance, dt.difficulty FROM dictionary_term dt "
        // :
        // "SELECT DISTINCT dt.id, dt.title, dt.disambiguation, dt.dictionary_map_id FROM dictionary_term dt "
        ;
        std::string sql_start_count = "SELECT COUNT(*) FROM dictionary_term dt ";
        std::string sql_where_and_joins;
        std::vector<BindValue> binders;

        bool first_where = true;

        // joins
        if (q.tag_id > 0)
            sql_where_and_joins += " JOIN dictionary_tag dtag ON dtag.dictionary_term_id = dt.id ";

        if (!q.flag_title.empty())
            sql_where_and_joins += " JOIN dictionary_flag df ON df.dictionary_term_id = dt.id ";

        if (!q.alias_alias.empty() || !q.alias_contains.empty())
            sql_where_and_joins += " JOIN dictionary_term_alias da ON da.dictionary_term_id = dt.id ";

        if (q.link_from_term_id > 0)
            sql_where_and_joins += " JOIN dictionary_link dl_from ON dl_from.to_dictionary_term_id = dt.id ";

        if (q.link_to_term_id > 0)
            sql_where_and_joins += " JOIN dictionary_link dl_to ON dl_to.from_dictionary_term_id = dt.id ";

        if (!q.note_contains.empty())
            sql_where_and_joins += " JOIN dictionary_note dn ON dn.dictionary_term_id = dt.id ";

        if (q.index_id > 0)
            sql_where_and_joins += " JOIN dictionary_index di ON di.dictionary_term_id = dt.id ";

        if (q.source_id > 0)
            sql_where_and_joins += " JOIN dictionary_source ds ON ds.dictionary_term_id = dt.id ";

        bool understandings_enabled =
            !q.understandings.empty() &&
            !(q.understandings.size() == 1 && q.understandings[0] == UnderstandingLevel::Any);

        if (understandings_enabled)
        {
            sql_where_and_joins +=
                " LEFT JOIN dictionary_term_understanding du "
                " ON du.dictionary_term_id = dt.id AND du.user_id = ? ";
            binders.push_back(user_id);
        }


        bool sort_next_review = q.sort == Sort::NextReview;
        if (sort_next_review)
        {
            sql_where_and_joins += " LEFT JOIN dictionary_state_18 state ON state.dictionary_term_id = dt.id ";
            sql_where_and_joins += " AND state.user_id = ? ";
            binders.push_back(user_id);
        }

        // pinned
        if (q.pinned_only)
        {
            sql_where_and_joins +=
                " JOIN dictionary_pinned_term dpt ON dpt.dictionary_term_id = dt.id ";
            append_where(sql_where_and_joins, first_where);
            sql_where_and_joins += "dpt.user_id = ?";
            binders.push_back(user_id);
        }

        if (!any_map)
        {
            // mandatory
            append_where(sql_where_and_joins, first_where);
            sql_where_and_joins += "dt.dictionary_map_id = ?";
            binders.push_back(dictionary_map_id);
        }

        // title filters
        if (!q.title_contains.empty())
        {
            append_where(sql_where_and_joins, first_where);
            sql_where_and_joins += "dt.title LIKE ?";
            binders.push_back("%" + q.title_contains + "%");
        }

        if (!q.title_starts_with.empty())
        {
            append_where(sql_where_and_joins, first_where);
            sql_where_and_joins += "dt.title LIKE ?";
            binders.push_back(q.title_starts_with + "%");
        }

        // definition
        if (!q.definition_contains.empty())
        {
            append_where(sql_where_and_joins, first_where);
            sql_where_and_joins += "dt.definition LIKE ?";
            binders.push_back("%" + q.definition_contains + "%");
        }

        // status
        if (!q.statuses.empty() && !(q.statuses.size() == 1 && q.statuses[0] == TermStatus::Any))
        {
            append_where(sql_where_and_joins, first_where);
            sql_where_and_joins += "dt.status IN (";
            for (size_t i = 0; i < q.statuses.size(); ++i)
            {
                if (i) sql_where_and_joins += ",";
                sql_where_and_joins += "?";
                auto status = q.statuses[i];
                auto status_int = term_status_to_int(status);
                if (status_int == -1) continue;
                binders.push_back(status_int);
            }
            sql_where_and_joins += ")";
        }

        if (understandings_enabled)
        {
            append_where(sql_where_and_joins, first_where);
            sql_where_and_joins += "COALESCE(du.level, 0) IN (";

            for (size_t i = 0; i < q.understandings.size(); ++i)
            {
                if (i) sql_where_and_joins += ",";
                sql_where_and_joins += "?";

                auto u = q.understandings[i];
                auto u_int = understanding_level_to_int(u);
                if (u_int == -1) continue;

                binders.push_back(u_int);
            }

            sql_where_and_joins += ")";
        }

        // difficulty
        if (!(q.difficulty_easy && q.difficulty_medium && q.difficulty_hard))
        {
            append_where(sql_where_and_joins, first_where);
            sql_where_and_joins += "dt.difficulty IN (";
            bool first = true;
            if (q.difficulty_easy)
            {
                sql_where_and_joins += std::to_string(difficulty_to_int(Difficulty::Easy));
                first = false;
            }
            if (q.difficulty_medium)
            {
                if (!first) sql_where_and_joins += ",";
                sql_where_and_joins += std::to_string(difficulty_to_int(Difficulty::Medium));
                first = false;
            }
            if (q.difficulty_hard)
            {
                if (!first) sql_where_and_joins += ",";
                sql_where_and_joins += std::to_string(difficulty_to_int(Difficulty::Hard));
            }
            sql_where_and_joins += ")";
        }

        // importance
        if (!(q.importance_low && q.importance_medium && q.importance_high))
        {
            append_where(sql_where_and_joins, first_where);
            sql_where_and_joins += "dt.importance IN (";
            bool first = true;
            if (q.importance_low)
            {
                sql_where_and_joins += std::to_string(importance_to_int(Importance::Low));
                first = false;
            }
            if (q.importance_medium)
            {
                if (!first) sql_where_and_joins += ",";
                sql_where_and_joins += std::to_string(importance_to_int(Importance::Medium));
                first = false;
            }
            if (q.importance_high)
            {
                if (!first) sql_where_and_joins += ",";
                sql_where_and_joins += std::to_string(importance_to_int(Importance::High));
            }
            sql_where_and_joins += ")";
        }

        // is_for_repetition
        if (q.is_for_repetition != IsForRepetitionMode::Any)
        {
            if (q.is_for_repetition == IsForRepetitionMode::ForRepetition) {
                append_where(sql_where_and_joins, first_where);
                sql_where_and_joins += "dt.is_for_repetition = 1";
            }
            if (q.is_for_repetition == IsForRepetitionMode::NotForRepetition) {
                append_where(sql_where_and_joins, first_where);
                sql_where_and_joins += "dt.is_for_repetition = 0";
            }
        }
        // is_root
        if (q.root_only)
        {
            append_where(sql_where_and_joins, first_where);
            sql_where_and_joins += "dt.is_root = 1";
        }
        // tag
        if (q.tag_id > 0)
        {
            append_where(sql_where_and_joins, first_where);
            sql_where_and_joins += "dtag.dictionary_tag_type_id = ?";
            binders.push_back(q.tag_id);
        }

        // flag
        if (!q.flag_title.empty())
        {
            append_where(sql_where_and_joins, first_where);
            sql_where_and_joins += "df.title = ? AND df.user_id = ?";
            binders.push_back(q.flag_title);
            binders.push_back(user_id);
        }

        // alias
        if (!q.alias_alias.empty())
        {
            append_where(sql_where_and_joins, first_where);
            sql_where_and_joins += "da.alias = ?";
            binders.push_back("%" + q.alias_alias + "%");
        }
        if (!q.alias_contains.empty())
        {
            append_where(sql_where_and_joins, first_where);
            sql_where_and_joins += "da.alias LIKE ?";
            binders.push_back("%" + q.alias_contains + "%");
        }

        // note
        if (!q.note_contains.empty())
        {
            append_where(sql_where_and_joins, first_where);
            sql_where_and_joins += "((dn.title LIKE ?) OR (dn.content LIKE ?))";
            binders.push_back("%" + q.note_contains + "%");
            binders.push_back("%" + q.note_contains + "%");
        }

        // index
        if (q.index_id > 0)
        {
            append_where(sql_where_and_joins, first_where);
            sql_where_and_joins += "di.dictionary_index_type_id = ?";
            binders.push_back(q.index_id);
        }

        // source
        if (q.source_id > 0)
        {
            append_where(sql_where_and_joins, first_where);
            sql_where_and_joins += "ds.dictionary_source_type_id = ?";
            binders.push_back(q.source_id);
        }
        // source
        if (q.source_id > 0)
        {
            append_where(sql_where_and_joins, first_where);
            sql_where_and_joins += "ds.dictionary_source_type_id = ?";
            binders.push_back(q.source_id);
        }

        static const std::unordered_map<DictionaryItem, std::string> dictionary_item_tables = {
            {DictionaryItem::Notes, "dictionary_note dn"},
            {DictionaryItem::Aliases, "dictionary_term_alias da"},
            {DictionaryItem::Tags, "dictionary_tag dtag"},
            {DictionaryItem::Flags, "dictionary_flag df"},
            {DictionaryItem::Sources, "dictionary_source ds"},
            {DictionaryItem::Indexes, "dictionary_index di"}
        };

        // has items
        if (!q.has_items.empty())
        {
            // common EXISTS patterns

            for (const auto& item : q.has_items)
            {
                std::string condition;

                // --- special cases ---
                if (item == DictionaryItem::Definition)
                {
                    condition = "(dt.definition IS NOT NULL AND dt.definition != '')";
                }
                else if (item == DictionaryItem::Links)
                {
                    condition =
                        "EXISTS (SELECT 1 FROM dictionary_link dl "
                        "WHERE dl.from_dictionary_term_id = dt.id "
                        "   OR dl.to_dictionary_term_id = dt.id)";
                }
                // --- generic EXISTS ---
                else if (auto it = dictionary_item_tables.find(item); it != dictionary_item_tables.end())
                {
                    const std::string& table = it->second;
                    const std::string alias = table.substr(table.find(' ') + 1);

                    condition =
                        "EXISTS (SELECT 1 FROM " + table +
                        " WHERE " + alias + ".dictionary_term_id = dt.id)";
                }

                if (!condition.empty())
                {
                    append_where(sql_where_and_joins, first_where);
                    sql_where_and_joins += condition;
                }
            }
        }

        // missing items
        if (!q.missing_items.empty())
        {
            // common NOT EXISTS patterns

            for (const auto& item : q.missing_items)
            {
                std::string condition;

                // --- special cases ---
                if (item == DictionaryItem::Definition)
                {
                    condition = "(dt.definition IS NULL OR dt.definition = '')";
                }
                else if (item == DictionaryItem::Links)
                {
                    condition =
                        "NOT EXISTS (SELECT 1 FROM dictionary_link dl "
                        "WHERE dl.from_dictionary_term_id = dt.id "
                        "   OR dl.to_dictionary_term_id = dt.id)";
                }
                // --- generic NOT EXISTS ---
                else if (auto it = dictionary_item_tables.find(item); it != dictionary_item_tables.end())
                {
                    condition =
                        "NOT EXISTS (SELECT 1 FROM " + it->second +
                        " WHERE " + it->second.substr(it->second.find(' ') + 1) +
                        ".dictionary_term_id = dt.id)";
                }

                // append only if we really added a condition
                if (!condition.empty())
                {
                    append_where(sql_where_and_joins, first_where);
                    sql_where_and_joins += condition;
                }
            }
        }

        // created
        if (q.created != TimeRange::Any)
        {
            if (q.created == TimeRange::Never)
            {
                append_where(sql_where_and_joins, first_where);
                sql_where_and_joins +=
                    "(dt.created_at IS NULL)";
            }
            else
            {
                bool negated = is_negative_time_range(q.created);

                i64 threshold_ms = threshold_from_time_range(q.created, now_ms);

                if (threshold_ms > 0)
                {
                    append_where(sql_where_and_joins, first_where);

                    if (!negated)
                    {
                        sql_where_and_joins +=
                            "dt.created_at >= ?";
                    }
                    else
                    {
                        sql_where_and_joins +=
                            "(dt.created_at < ? OR dt.created_at IS NULL)";
                    }

                    binders.push_back(threshold_ms);
                }
            }
        }
        // updated
        if (q.updated != TimeRange::Any)
        {
            if (q.updated == TimeRange::Never)
            {
                append_where(sql_where_and_joins, first_where);
                sql_where_and_joins +=
                    "(dt.updated_at IS NULL OR dt.updated_at = dt.created_at)";
            }
            else
            {
                bool negated = is_negative_time_range(q.updated);
                i64 threshold_ms = threshold_from_time_range(q.updated, now_ms);

                if (threshold_ms > 0)
                {
                    append_where(sql_where_and_joins, first_where);

                    if (!negated)
                    {
                        sql_where_and_joins += "dt.updated_at >= ?";
                    }
                    else
                    {
                        sql_where_and_joins +=
                            "(dt.updated_at < ? OR dt.updated_at IS NULL)";
                    }

                    binders.push_back(threshold_ms);
                }
            }
        }

        // visited
        if (q.visited != TimeRange::Any)
        {
            if (q.visited == TimeRange::Never)
            {
                append_where(sql_where_and_joins, first_where);
                sql_where_and_joins +=
                    "NOT EXISTS (SELECT 1 FROM dictionary_term_visit dtv "
                    "WHERE dtv.dictionary_term_id = dt.id "
                    "AND dtv.user_id = ?)";
                binders.push_back(user_id);
            }
            else
            {
                bool negated = is_negative_time_range(q.visited);
                i64 threshold_ms = threshold_from_time_range(q.visited, now_ms);

                if (threshold_ms > 0)
                {
                    append_where(sql_where_and_joins, first_where);

                    if (!negated)
                    {
                        sql_where_and_joins +=
                            "EXISTS (SELECT 1 FROM dictionary_term_visit dtv "
                            "WHERE dtv.dictionary_term_id = dt.id "
                            "AND dtv.user_id = ? "
                            "AND dtv.created_at >= ?)";
                    }
                    else
                    {
                        sql_where_and_joins +=
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

        // reviewed
        if (q.reviewed != TimeRange::Any)
        {
            if (q.reviewed == TimeRange::Never)
            {
                append_where(sql_where_and_joins, first_where);
                sql_where_and_joins +=
                    "NOT EXISTS (SELECT 1 FROM dictionary_state_18 ds "
                    "WHERE ds.dictionary_term_id = dt.id "
                    "AND ds.user_id = ?)";
                binders.push_back(user_id);
            }
            else
            {
                bool negated = is_negative_time_range(q.reviewed);
                i64 threshold_ms = threshold_from_time_range(q.reviewed, now_ms);

                if (threshold_ms > 0)
                {
                    append_where(sql_where_and_joins, first_where);

                    if (!negated)
                    {
                        sql_where_and_joins +=
                            "EXISTS (SELECT 1 FROM dictionary_state_18 ds "
                            "WHERE ds.dictionary_term_id = dt.id "
                            "AND ds.user_id = ? "
                            "AND ds.updated_at >= ?)";
                    }
                    else
                    {
                        sql_where_and_joins +=
                            "NOT EXISTS (SELECT 1 FROM dictionary_state_18 ds "
                            "WHERE ds.dictionary_term_id = dt.id "
                            "AND ds.user_id = ? "
                            "AND ds.updated_at >= ?)";
                    }

                    binders.push_back(user_id);
                    binders.push_back(threshold_ms);
                }
            }
        }

        bool require_definition = false;
        for (auto& e : q.has_items)
        {
            if (e == DictionaryItem::Definition)
            {
                require_definition = true;
                break;
            }
        }

        // repetition
        if (!(q.repetition_due && q.repetition_not_due && q.repetition_never))
        {
            append_where(sql_where_and_joins, first_where);
            sql_where_and_joins += " ( ";

            bool first_rep = true;

            auto add_or = [&]()
            {
                if (!first_rep)
                    sql_where_and_joins += " OR ";
                first_rep = false;
            };

            // -------- due --------
            if (q.repetition_due)
            {
                add_or();
                sql_where_and_joins += R"(
EXISTS (
    SELECT 1
    FROM dictionary_state_18 s
    WHERE s.dictionary_term_id = dt.id
      AND s.user_id = ?
      AND s.next_review <= ?
)
)";
                binders.push_back(user_id);
                binders.push_back(now_ms);
            }

            // -------- not due --------
            if (q.repetition_not_due)
            {
                add_or();
                sql_where_and_joins += R"(
EXISTS (
    SELECT 1
    FROM dictionary_state_18 s
    WHERE s.dictionary_term_id = dt.id
      AND s.user_id = ?
      AND s.next_review > ?
)
)";
                binders.push_back(user_id);
                binders.push_back(now_ms);
            }

            // -------- never --------
            if (q.repetition_never)
            {
                add_or();
                sql_where_and_joins += R"(
NOT EXISTS (
    SELECT 1
    FROM dictionary_state_18 s
    WHERE s.dictionary_term_id = dt.id
      AND s.user_id = ?
)
)";
                binders.push_back(user_id);
            }

            sql_where_and_joins += " ) ";

            // optional: require definition
            if (require_definition)
            {
                append_where(sql_where_and_joins, first_where);
                sql_where_and_joins += " TRIM(dt.definition) <> '' ";
            }
        }

        std::string sql_sort;
        switch (q.sort)
        {
        case Sort::Title:
            sql_sort = "dt.title";
            break;

        case Sort::CreatedAt:
            sql_sort = "dt.created_at";
            break;

        case Sort::UpdatedAt:
            sql_sort = "dt.updated_at";
            break;

        case Sort::Status:
            sql_sort = "dt.status";
            break;

        case Sort::Difficulty:
            sql_sort = "dt.difficulty";
            break;

        case Sort::Importance:
            sql_sort = "dt.importance";
            break;

        case Sort::NextReview:
            sql_sort = "coalesce(state.next_review, 18446744073709551615)";
            break;

        case Sort::Random:
            sql_sort = "random()";
            break;

        case Sort::None:
        default:
            break;
        }

        if (!sql_sort.empty())
        {
            sql_sort = " ORDER BY " + sql_sort;
            if (q.sort != Sort::Random && q.order != Order::None)
            {
                sql_sort += " ";
                sql_sort += order_to_string(q.order);
            }
        }

        static std::string sql_limit_offset = " LIMIT ? OFFSET ?";

        std::string sql_page;
        std::string sql_count;

        essential::debug << "###sql_current_page### " + sql_where_and_joins << essential::commit;
        try
        {
            SQLite::Database db(SQLITE_FILE_NAME, SQLite::OPEN_READONLY);

            {
                sql_page = sql_start_page + sql_where_and_joins;
                sql_page += sql_sort;
                sql_page += sql_limit_offset;
                SQLite::Statement stmt(db, sql_page);

                int i = 0;
                for (const BindValue& b : binders)
                {
                    switch (b.get_type())
                    {
                    case TEXT: stmt.bind(++i, b.get_text());
                        break;
                    case NUMBER: stmt.bind(++i, b.get_number());
                        break;
                    default: throw std::runtime_error(std::string("Unknown type: ") + std::to_string(b.get_type()));
                    }
                }

                stmt.bind(++i, page_size);
                stmt.bind(++i, offset);

                nlohmann::json arr = nlohmann::json::array();
                while (stmt.executeStep())
                {
                    // if (any_map)
                    {
                        arr.push_back({
                            {"id", stmt.getColumn(0).getInt64()},
                            {"title", stmt.getColumn(1).getString()},
                            {"disambiguation", stmt.getColumn(2).getString()},
                            {"map_id", stmt.getColumn(3).getInt64()},
                            {"created_at", stmt.getColumn(4).getInt64()},
                            {"updated_at", stmt.getColumn(5).getInt64()},
                            {"status", stmt.getColumn(6).getInt()},
                            {"importance", stmt.getColumn(7).getInt()},
                            {"difficulty", stmt.getColumn(8).getInt()}
                        });
                    }
                    // else
                    // {
                    //     arr.push_back({
                    //         {"id", stmt.getColumn(0).getInt64()},
                    //         {"title", stmt.getColumn(1).getString()},
                    //         {"disambiguation", stmt.getColumn(2).getString()},
                    //         {"map_id", stmt.getColumn(3).getInt64()},
                    //     });
                    // }
                }
                response["results"] = arr;
            }

            {
                sql_count = sql_start_count + sql_where_and_joins;

                SQLite::Statement stmt(db, sql_count);

                int i = 0;
                for (const BindValue& b : binders)
                {
                    switch (b.get_type())
                    {
                    case TEXT: stmt.bind(++i, b.get_text());
                        break;
                    case NUMBER: stmt.bind(++i, b.get_number());
                        break;
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
            response["sql_page"] = sql_page;
            response["sql_count"] = sql_count;
            plugins::core::models::Error error = plugins::core::ErrorBuilder(
                                                     "FindDictionaryTermsViaAdvancedSearchSQLiteQuery failed",
                                                     plugins::core::enums::ErrorSeverity::Error,
                                                     plugins::core::enums::ErrorOrigin::Trigger,
                                                     plugins::core::enums::ExecutionLayer::Db)
                                                 .plugin("dictionary")
                                                 .operation(essential::Crudl::List)
                                                 .cpp(
                                                     CPP_NAMESPACE,
                                                     CPP_CLASS,
                                                     CPP_SYMBOL
                                                 )
                                                 .sql(sql_where_and_joins + "; " + sql_count)
                                                 .user(user_id)
                                                 .exception(e)
                                                 .build();
            optional_error.emplace(error);
            response["error"] = error.create_message_for_user();
        }

        return response;
    }
}
