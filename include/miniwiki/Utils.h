//
// Created by robertvokac on 7/31/25.
//

#ifndef UTILS_H
#define UTILS_H
#include <string>
#include <vector>

#include "Global.h"
#include "Helper.h"
#include "models/BaseEntity.h"
#include "SQLiteCpp/Statement.h"

namespace MiniWiki
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

        static void clearScreen();
        static void log(const char* message);
        static void log(const std::string& message);
        static void trace(const char* message);
        static bool ask_yes_no(const std::string& question);
        static std::string editTextInEditor(const std::string& inputText, const std::string& editorPath);

        // Convert single letter 'a'-'z' to number 0-25
        static int letterToDecimal(char letter);

        // Convert number 0-25 to single letter 'a'-'z'
        static char decimalToLetter(int number);

        // Convert base-26 string (a-z) to decimal integer
        // "a" -> 0, "b" -> 1, ..., "z" -> 25, "aa" -> 26, etc.
        static int base26ToDecimal(const std::string& text);

        // Convert decimal integer to base-26 string (a-z)
        static std::string decimalToBase26(int number);
        static std::string remove_semicolon(std::string str);

        static void sqlite_exec(SQLite::Statement& query);


        /**
     * Converts a note identifier string into a vector of string components.
     * The note ID is split into alternating sections of digits and letters.
     *
     * @param note_id The note identifier string to be split. This string must start with a digit.
     *                An empty string or one that doesn't start with a digit will result in an exception.
     *
     * @return A vector containing the split components of the note ID in the order of their appearance.
     *         Each component will either be a series of digits or a series of letters.
     *
     * @throws std::runtime_error If the provided note_id is empty or does not start with a digit.
     */
        static std::vector<std::string> note_id_to_vector(const std::string& note_id);
        static std::string vector_to_note_id(const std::vector<std::string>& vector);
        static std::string next_note_id(const std::string& parent_note_id, const std::string& youngest_child_note_id);
        static bool get_note_id_from_args(std::string& current_path, const std::string& args, std::string& used_path);
        static std::vector<std::string> split_with_quotes(const std::string& input);

        static str generate_insert_sql(const std::string& table_name, const std::vector<const char*>& columns, bool auto_increment = false);
        static void fill_sqlite_query(SQLite::Statement& query, const entity_fields& values, bool auto_increment = false);

    };
} // MiniWiki

#endif //UTILS_H
