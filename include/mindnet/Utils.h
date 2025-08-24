//
// Created by robertvokac on 7/31/25.
//

#ifndef UTILS_H
#define UTILS_H
#include <string>
#include <vector>

#include "Global.h"
#include "Helper.h"
#include "http/QueryParams.h"
#include "models/misc/BaseModel.h"
#include "SQLiteCpp/Statement.h"

namespace mindnet
{
    class Utils
    {
    private:
        Utils() = delete;
        Utils(const Utils&) = delete;
        Utils& operator=(const Utils&) = delete;

    public:
        static long long currentTimestamp();
        static long long currentUnixTimestamp();
        static std::string unixToFormattedString(ll unixTimestamp);
        static str print_current_timestamp();
        // static ll formattedStringToUnix(const std::string& formattedTime);

        // Convert single letter 'a'-'z' to number 0-25
        static int letterToDecimal(char letter);

        // Convert number 0-25 to single letter 'a'-'z'
        static char decimalToLetter(int number);

        // Convert base-26 string (a-z) to decimal integer
        // "a" -> 0, "b" -> 1, ..., "z" -> 25, "aa" -> 26, etc.
        static int base26ToDecimal(const std::string& text);

        // Convert decimal integer to base-26 string (a-z)
        static std::string decimalToBase26(int number);

        static void sqlite_exec(SQLite::Statement& query);


        static std::vector<std::string> split_with_quotes(const std::string& input);

        static str generate_select_one_sql(const std::string& table_name);
        static str generate_select_all_sql(const std::string& table_name, const http::QueryParams& query_params, bool count = false);
        static str generate_select_count_sql(const std::string& table_name, const http::QueryParams& query_params);

        static str generate_insert_sql(const models::misc::ModelDefinition& definition);
        static str generate_update_sql(const models::misc::ModelDefinition& definition);
        static str generate_delete_sql(const models::misc::ModelDefinition& definition);

        static void fill_sqlite_query(SQLite::Statement& query, const entity_fields& values,
                                      bool auto_increment = false);
    };

    enum EscapeColor
    {
        Black = 0, Red = 1, Green = 2, Yellow = 3, Blue = 4, Magenta = 5, Cyan = 6, White = 7,
        BrightBlack = 8, BrightRed = 9, BrightGreen = 10, BrightYellow = 11, BrightBlue = 12, BrightMagenta = 13, BrightCyan = 14, BrightWhite = 15,
    };

    struct TextColour
    {
        int foreground = -1;
        int background = -1;
    };
    inline void set_text_colour(TextColour colour)
    {
        if (colour.foreground >=0) printf("\033[38;5;%dm", colour.foreground);
        if (colour.background >=0) printf("\033[48;5;%dm", colour.background);
    }

    inline void set_text_colour(EscapeColor foreground, EscapeColor background)
    {
        set_text_colour({foreground, background});
    }
    inline void set_text_colour(EscapeColor foreground)
    {
        set_text_colour({foreground, -1});
    }

}

#endif //UTILS_H
