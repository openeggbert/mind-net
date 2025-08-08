//
// Created by robertvokac on 7/31/25.
//

#include "NoteBox/Utils.h"

#include <cstdlib>
#include <ctime>
#include <iostream>
#include <string>
#include <fstream>
#include <random>
#include <filesystem>
#include <stdexcept>
#include <iomanip>

namespace NoteBox
{
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

    std::string Utils::unixToFormattedString(ll unixTimestamp) {
        std::time_t rawTime = static_cast<std::time_t>(unixTimestamp);
        std::tm* timeInfo = std::localtime(&rawTime);

        char buffer[16];
        std::strftime(buffer, sizeof(buffer), "%Y%m%d%H%M%S", timeInfo);
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
    } // NoteBox
    void Utils::trace(const char* message)
    {
        //std::cout << "[TRACE] " << message << std::endl;
    }

    bool Utils::ask_yes_no(const std::string& question)
    {
        std::cout << question << " [y/n] ";
        char choice;
        std::cin >> choice;
        return choice == 'y';
    }


    std::string Utils::editTextInEditor(const std::string& inputText, const std::string& editorPath) {
        // Generate a random filename in /tmp/
        std::string filename = "/tmp/tmp_edit_XXXXXX.txt";
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(0, 25);

        for (auto& ch : filename) {
            if (ch == 'X') {
                ch = 'a' + dis(gen);
            }
        }

        // Write input text to the file
        {
            std::ofstream outFile(filename);
            if (!outFile) {
                throw std::runtime_error("Failed to create temporary file: " + filename);
            }
            outFile << inputText;
        }

        // Launch editorPath in blocking mode
        std::string command = editorPath + std::string(" ") + filename;
        int result = std::system(command.c_str());
        if (result != 0) {
            std::filesystem::remove(filename);
            throw std::runtime_error(editorPath + " returned non-zero exit code.");
        }

        // Read modified contents
        std::ifstream inFile(filename);
        if (!inFile) {
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
            throw std::runtime_error("Note id must start with a digit");

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
}
