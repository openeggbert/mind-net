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

}
