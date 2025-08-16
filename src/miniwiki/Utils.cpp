//
// Created by robertvokac on 7/31/25.
//

#include "miniwiki/Utils.h"

#include <cstdlib>
#include <ctime>
#include <iostream>
#include <string>
#include <fstream>
#include <random>
#include <filesystem>
#include <stdexcept>
#include <regex>

#include "miniwiki/Global.h"
#include "miniwiki/persistence/Impl/Sqlite/SqliteFileName.h"
#include "SQLiteCpp/Database.h"

namespace miniwiki
{
    namespace Entity
    {
        struct BaseEntity;
    }

    long long Utils::currentTimestamp()
    {
        time_t now = time(nullptr);
        struct tm* now2 = localtime(&now);
        char buffer[80];
        strftime(buffer, sizeof(buffer), "%Y%m%d%H%M%S", now2);
        return atoll(buffer);
    }

    long long Utils::currentUnixTimestamp()
    {
        return static_cast<long long>(std::time(nullptr));
    }

    std::string Utils::unixToFormattedString(ll unixTimestamp)
    {
        if (unixTimestamp == 0)
        {
            return "none";
        }
        std::time_t rawTime = static_cast<std::time_t>(unixTimestamp);
        std::tm* timeInfo = std::localtime(&rawTime);

        char buffer[21];
        std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", timeInfo);
        return std::string(buffer);
    }


    // ll Utils::formattedStringToUnix(const std::string& formattedTime)
    // {
    //     std::tm timeInfo = {};
    //     std::istringstream ss(formattedTime);
    //     ss >> std::get_time(&timeInfo, "%Y%m%d%H%M%S");
    //
    //     if (ss.fail())
    //     {
    //         throw std::runtime_error("Invalid time format");
    //     }
    //
    //     timeInfo.tm_isdst = -1; // Let system determine DST
    //     auto time = std::mktime(&timeInfo);
    //     if (time == -1)
    //     {
    //         throw std::runtime_error("Failed to convert time");
    //     }
    //     return static_cast<long long>(time);
    // }


    void Utils::clearScreen()
    {
        std::cout << "\033[2J\033[H";
    }

    void Utils::log(const std::string& msg)
    {
        std::cout << "[INFO] " << msg << std::endl;
    }

    void Utils::log(const char* message)
    {
        log(std::string(message));
    } // MiniWiki
    void Utils::trace(const char* message)
    {
        //std::cout << "[TRACE] " << message << std::endl;
    }

    bool Utils::ask_yes_no(const std::string& question)
    {
        std::cout << question << " [y/n] ";
        char choice;
        std::cin >> choice;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        return choice == 'y';
    }


    std::string Utils::editTextInEditor(const std::string& inputText, const std::string& editorPath)
    {
        // Generate a random filename in /tmp/
        std::string filename = "/tmp/tmp_edit_XXXXXX.txt";
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(0, 25);

        for (auto& ch : filename)
        {
            if (ch == 'X')
            {
                ch = 'a' + dis(gen);
            }
        }

        // Write input text to the file
        {
            std::ofstream outFile(filename);
            if (!outFile)
            {
                throw std::runtime_error("Failed to create temporary file: " + filename);
            }
            outFile << inputText;
        }

        // Launch editorPath in blocking mode
        std::string command = editorPath + std::string(" ") + filename;
        int result = std::system(command.c_str());
        if (result != 0)
        {
            std::filesystem::remove(filename);
            throw std::runtime_error(editorPath + " returned non-zero exit code.");
        }

        // Read modified contents
        std::ifstream inFile(filename);
        if (!inFile)
        {
            std::filesystem::remove(filename);
            throw std::runtime_error("Failed to read temporary file after editing.");
        }
        std::string modifiedText((std::istreambuf_iterator<char>(inFile)),
                                 std::istreambuf_iterator<char>());

        // Remove the temporary file
        std::filesystem::remove(filename);

        return modifiedText;
    }


    std::vector<std::string> Utils::note_id_to_vector(const std::string& note_id)
    {
        if (note_id.empty())
            throw std::runtime_error("Empty note id is not valid");
        if (!std::isdigit(note_id[0]))
            throw std::runtime_error("Note id must start with a digit: " + note_id);

        std::vector<std::string> result;
        std::string current;
        bool is_digit = std::isdigit(note_id[0]);

        for (char c : note_id)
        {
            if (std::isdigit(c) == is_digit)
            {
                current += c;
            }
            else
            {
                result.push_back(current);
                current = c;
                is_digit = !is_digit;
            }
        }

        if (!current.empty())
            result.push_back(current);

        return result;
    }

    std::string Utils::vector_to_note_id(const std::vector<std::string>& vector)
    {
        if (vector.empty())
            return "";

        std::string result;
        for (const auto& part : vector)
        {
            result += part;
        }
        return result;
    }

    // Convert single letter 'a'-'z' to number 0-25
    int Utils::letterToDecimal(char letter)
    {
        if (letter < 'a' || letter > 'z')
        {
            throw std::invalid_argument("letterToDecimal: input must be a-z");
        }
        return letter - 'a';
    }

    // Convert number 0-25 to single letter 'a'-'z'
    char Utils::decimalToLetter(int number)
    {
        if (number < 0 || number > 25)
        {
            throw std::out_of_range("decimalToLetter: input must be 0-25");
        }
        return static_cast<char>('a' + number);
    }

    // Convert base-26 string (a-z) to decimal integer
    // "a" -> 0, "b" -> 1, ..., "z" -> 25, "aa" -> 26, etc.
    int Utils::base26ToDecimal(const std::string& text)
    {
        if (text.empty())
            throw std::invalid_argument("Empty string not allowed");

        for (char c : text)
        {
            if (c < 'a' || c > 'z')
                throw std::invalid_argument("Only lowercase a-z allowed");
        }

        int len = static_cast<int>(text.size());
        int value = 0;

        // Step 1: Add all strings with fewer letters
        int shorter_count = 26;
        for (int i = 1; i < len; ++i)
        {
            value += shorter_count;
            shorter_count *= 26;
        }

        // Step 2: Calculate index within same-length strings
        int offset = 0;
        for (char c : text)
        {
            offset = offset * 26 + letterToDecimal(c);
        }

        return value + offset;
    }


    // Convert decimal integer to base-26 string (a=0, b=1, ..., z=25, aa=26, etc.)
    std::string Utils::decimalToBase26(int number)
    {
        if (number < 0)
            throw std::invalid_argument("Negative numbers not allowed");

        std::string result;
        int length = 1;

        // Find how many letters are needed
        int count = 26;
        int remaining = number;
        while (remaining >= count)
        {
            remaining -= count;
            length++;
            count *= 26;
        }

        // Build string from remaining number
        for (int i = 0; i < length; ++i)
        {
            int power = length - i - 1;
            int idx = remaining / static_cast<int>(std::pow(26, power));
            result.push_back(decimalToLetter(idx));
            remaining %= static_cast<int>(std::pow(26, power));
        }

        return result;
    }

    std::string Utils::remove_semicolon(std::string str)
    {
        int index = 0;
        for (int i = str.size() - 1; i >= 0; i--)
        {
            if (str[i] == ';')
            {
                index = i;
                break;
            }
        }
        if (index == 0)
        {
            return str;
        }
        else
        {
            return str.substr(0, index) + str.substr(index + 1, str.size() - index - 1);
        }
    }

    void Utils::sqlite_exec(SQLite::Statement& query)
    {
        try
        {
            query.exec();
        }
        catch (SQLite::Exception& e)
        {
            err << "Exception during SQLite statement execution: " << e.what() << std::endl;
            throw std::runtime_error(e.what());
        }
    }


    /**
     * Generates new Note ID in the ZettelKasten system.
     * The note ID is composed of alternating numbers and lowercase letters.
     * Root notes are just numbers (1, 2, 3...).
     * Child notes append letters and numbers to their parent's ID.
     * For example: 1a1, 1a2, 1b1, 2a1, etc.
     *
     * @param parent_note_id The ID of the parent note. Empty string means root level.
     * @param youngest_child_note_id The ID of the youngest (most recently created) child note.
     *                              Empty string means no existing children.
     * @return The generated ID for the new note
     */
    std::string Utils::next_note_id(const std::string& parent_note_id, const std::string& youngest_child_note_id)
    {
        //std::cout << "Generating next note ID for parent note " << parent_note_id << " with youngest child " << youngest_child_note_id << std::endl;
        bool parent_is_root = parent_note_id.empty();
        bool parent_has_children = !youngest_child_note_id.empty();
        if (parent_is_root)
        {
            if (parent_has_children)
            {
                int i = stoi(youngest_child_note_id) + 1;
                return std::to_string(i);
            }
            return "1";
        }
        //parent is not root
        if (parent_has_children)
        {
            auto v = note_id_to_vector(youngest_child_note_id);
            auto last_part = v.back();
            v.pop_back();
            if (isdigit(last_part[0]))
            {
                last_part = std::to_string(stoi(last_part) + 1);
            }
            else
            {
                last_part = decimalToBase26(base26ToDecimal(last_part) + 1);
            }
            v.push_back(last_part);
            return vector_to_note_id(v);
        }
        else
        {
            auto v = note_id_to_vector(parent_note_id);
            bool digit = isdigit(v.back()[0]);
            return parent_note_id + (digit ? "a" : "1");
        }
    }

    /**
     * Determines which path to use based on provided arguments and current context.
     * If arguments are provided, the `used_path` will be set to those arguments.
     * If no arguments are provided, the current path will be used unless it is empty.
     * If the current path is empty, an error message will be logged, and the function will return true.
     *
     * @param current_path Reference to the current path. An empty string indicates the root directory.
     * @param args String containing path arguments. If not empty, this path will be prioritized.
     * @param used_path Reference where the resulting path (based on arguments or current path) will be stored.
     * @return Returns true if the current path is empty and no arguments are provided, indicating an error.
     *         Otherwise, returns false.
     */
    bool Utils::get_note_id_from_args(std::string& current_path, const std::string& args, std::string& used_path)
    {
        if (!args.empty())
        {
            used_path = args;
        }
        else
        {
            if (current_path.empty())
            {
                err << "Cannot run command, because the current path is the root directory" << std::endl;
                return true;
            }
            used_path = current_path;
        }
        return false;
    }

    std::vector<std::string> Utils::split_with_quotes(const std::string& input)
    {
        std::vector<std::string> result;
        std::string current;
        bool in_quotes = false;

        for (size_t i = 0; i < input.size(); ++i)
        {
            char c = input[i];

            if (c == '"')
            {
                in_quotes = !in_quotes;
            }
            else if (std::isspace(static_cast<unsigned char>(c)) && !in_quotes)
            {
                if (!current.empty())
                {
                    result.push_back(current);
                    current.clear();
                }
            }
            else
            {
                current += c;
            }
        }

        if (!current.empty())
        {
            result.push_back(current);
        }

        return result;
    }

    str Utils::generate_insert_sql(const std::string& table_name, const std::vector<const char*>& columns, bool auto_increment)
    {
        str sql = "INSERT INTO " + table_name + " (";
        for (int i = 0; i < columns.size(); ++i)
        {
            if (auto_increment && std::string(columns[i]) == "ID")
            {
                continue;
            }
            sql += columns[i];
            if (i < columns.size() - 1)
            {
                sql += ", ";
            }
        }
        sql += ") VALUES (";
        for (int i = 0; i < columns.size(); ++i)
        {
            if (auto_increment && std::string(columns[i]) == "ID")
            {
                continue;
            }
            sql += "?";
            if (i < columns.size() - 1)
            {
                sql += ", ";
            }
        }
        sql += ")";
        return sql;
    }

    template<class>
    inline constexpr bool always_false = false;

    void Utils::fill_sqlite_query(SQLite::Statement& query, const entity_fields& values, bool auto_increment)
    {
        std::size_t values_size = values.size();
        if (auto_increment) {values_size--;}
        if (values_size > static_cast<size_t>(query.getBindParameterCount())) {
            err << "values().size " << values_size << " > query.getBindParameterCount() " << static_cast<size_t>(query.getBindParameterCount()) << std::endl;
            throw std::out_of_range("More values provided than query parameters");
        }

        for (size_t i = 0; i < values.size(); ++i)
        {
            if (auto_increment && i == 0)
            {
                continue;
            }
            err << "binding index " << i << " with value " << std::endl;
            std::visit([&](auto&& val) -> void {
                using T = std::decay_t<decltype(val)>;

                if constexpr (std::is_same_v<T, std::string>) {
                    query.bind(static_cast<int>(i + 1 + (auto_increment ? -1 : 0)), val);
                }
                else if constexpr (std::is_same_v<T, int64_t>) {
                    query.bind(static_cast<int>(i + 1 + (auto_increment ? -1 : 0)), val);
                }
                else {
                    static_assert(always_false<T>, "Unsupported type in SqlValue");
                }
            }, values[i]);
        }
    }


}
