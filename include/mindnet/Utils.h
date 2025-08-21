//
// Created by robertvokac on 7/31/25.
//

#ifndef UTILS_H
#define UTILS_H
#include <string>
#include <vector>

#include "Global.h"
#include "Helper.h"
#include "models/BaseModel.h"
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
        // static ll formattedStringToUnix(const std::string& formattedTime);

        static void log(const char* message);
        static void log(const std::string& message);
        static void trace(const char* message);

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

        static str generate_insert_sql(const models::ModelDefinition& definition);
        static str generate_update_sql(const models::ModelDefinition& definition);
        static str generate_delete_sql(const models::ModelDefinition& definition);



        static void fill_sqlite_query(SQLite::Statement& query, const entity_fields& values,
                                      bool auto_increment = false);
    };
} // MiniWiki

#endif //UTILS_H
