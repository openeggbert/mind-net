//
// Created by robertvokac on 7/31/25.
//

#include "NoteBox/Utils.h"

#include <cstdlib>
#include <ctime>
#include <iostream>

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
}
