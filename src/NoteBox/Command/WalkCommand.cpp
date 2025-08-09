//
// Created by robertvokac on 8/4/25.
//

#include "NoteBox/Command/WalkCommand.h"
#include <iostream>
#include "NoteBox/Global.h"
#include <termios.h>
#include <unistd.h>
#include <string>
#include <cctype>   // tolower
#include <cerrno>
#include <cstring>  // strerror


namespace NoteBox::Command
{

    // RAII guard pro termios
    class TermiosGuard {
        termios oldt_ = {};
        bool enabled_ = false;
    public:
        void enableRaw() {
            if (enabled_) return;
            if (tcgetattr(STDIN_FILENO, &oldt_) == -1) {
                std::cerr << "tcgetattr failed: " << strerror(errno) << "\n";
                return;
            }
            termios raw = oldt_;
            raw.c_lflag &= ~(ICANON | ECHO);
            raw.c_cc[VMIN] = 1;
            raw.c_cc[VTIME] = 0;
            if (tcsetattr(STDIN_FILENO, TCSANOW, &raw) == -1) {
                std::cerr << "tcsetattr failed: " << strerror(errno) << "\n";
                return;
            }
            enabled_ = true;
        }

        void disable() {
            if (!enabled_) return;
            tcsetattr(STDIN_FILENO, TCSANOW, &oldt_);
            enabled_ = false;
        }

        ~TermiosGuard() { disable(); }
    };

    ssize_t readByte(unsigned char &out) {
        return read(STDIN_FILENO, &out, 1);
    }

    void show(Manager::NoteBoxManager& mgr, ICommandHelper* helper)
    {
        helper->execute(mgr, "show", "");
    }

    void WalkCommand::execute(Manager::NoteBoxManager& mgr, const std::string& args)
    {
        Utils::clearScreen();

        show(mgr, helper);

        TermiosGuard tg;
        tg.enableRaw();

        while (true)
        {

            unsigned char c;
            ssize_t n = readByte(c);
            if (n <= 0) {
                if (n == -1 && errno == EINTR) continue;
                break;
            }



            char choice = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
            std::cout << "Choice: " << choice << std::endl;


            switch (choice)
            {
            case 'e':
                {
                    helper->execute(mgr, "edit", "");
                    break;
                }
            case 'h':
                {
                    tg.disable();
                    std::string command;
                    std::cout << "Argument (empty shows all available commands): ";
                    std::getline(std::cin, command);
                    tg.enableRaw();
                    helper->execute(mgr, "help", "");
                    break;
                }
            case 's' :
            {
                helper->execute(mgr, "show", "");
                break;
            }
            case 'x':
                {
                    std::cout << "Exiting walking." << std::endl;
                    return;
                }
            default: err << "Unknown command: " << choice << "\n";
            }
        }
    }

    void WalkCommand::help()
    {
        std::cout << R"(
NAME
       walk

SYNOPSIS
       walk

DESCRIPTION
       Starts an interactive mode, where the user uses only letters.

EXAMPLES
       walk ... there is always on argument

)" << std::endl;
    }

    void WalkCommand::setCommandHelper(ICommandHelper* command_helper_)
    {
        helper = command_helper_;
    };

    ICommandHelper* WalkCommand::get_command_helper()
    {
        return helper;
    }
}
