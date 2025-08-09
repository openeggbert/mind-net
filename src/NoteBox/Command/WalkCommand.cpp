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
    class TermiosGuard
    {
        termios oldt_ = {};
        bool enabled_ = false;

    public:
        void enableRaw()
        {
            if (enabled_) return;
            if (tcgetattr(STDIN_FILENO, &oldt_) == -1)
            {
                std::cerr << "tcgetattr failed: " << strerror(errno) << "\n";
                return;
            }
            termios raw = oldt_;
            raw.c_lflag &= ~(ICANON | ECHO);
            raw.c_cc[VMIN] = 1;
            raw.c_cc[VTIME] = 0;
            if (tcsetattr(STDIN_FILENO, TCSANOW, &raw) == -1)
            {
                std::cerr << "tcsetattr failed: " << strerror(errno) << "\n";
                return;
            }
            enabled_ = true;
        }

        void disable()
        {
            if (!enabled_) return;
            tcsetattr(STDIN_FILENO, TCSANOW, &oldt_);
            enabled_ = false;
        }

        ~TermiosGuard() { disable(); }
    };

    ssize_t readByte(unsigned char& out)
    {
        return read(STDIN_FILENO, &out, 1);
    }

    void prompt_user_to_continue() {
        std::cout << "Press any key to continue." << std::endl;
        std::cin.get();
    }

    void show(Manager::NoteBoxManager& mgr, ICommandHelper* helper)
    {
        helper->execute(mgr, "show", "");
    }

    char getch() {
        char buf = 0;
        struct termios old = {0};
        if (tcgetattr(0, &old) < 0)
            perror("tcsetattr()");
        old.c_lflag &= ~ICANON;
        old.c_lflag &= ~ECHO;
        old.c_cc[VMIN] = 1;
        old.c_cc[VTIME] = 0;
        if (tcsetattr(0, TCSANOW, &old) < 0)
            perror("tcsetattr ICANON");
        if (read(0, &buf, 1) < 0)
            perror ("read()");
        old.c_lflag |= ICANON;
        old.c_lflag |= ECHO;
        if (tcsetattr(0, TCSADRAIN, &old) < 0)
            perror ("tcsetattr ~ICANON");
        return (buf);
    }

    void WalkCommand::execute(Manager::NoteBoxManager& mgr, const std::string& args)
    {


        TermiosGuard tg;
        tg.enableRaw();

        while (true)
        {
            Utils::clearScreen();
            show(mgr, helper);

            unsigned char c;
            ssize_t n = readByte(c);
            if (n <= 0)
            {
                if (n == -1 && errno == EINTR) continue;
                break;
            }

            char choice = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
            //std::cout << "Choice: " << choice << "\n" << std::flush;

            Utils::clearScreen();
            switch (choice)
            {
            case 'e':
                {

                    tg.disable();
                    Utils::clearScreen();
                    helper->execute(mgr, "edit", "");

                    tg.enableRaw();
                    break;
                }
            case 'h':
                {
                    tg.disable();
std::cout << R"(
e ... edit content
h ... show this help
i ... show info about current Note
r ... run a command
s ... show content of the current Note
q ... exit the Walking mode

)";
                    prompt_user_to_continue();
                    break;
                }
            case 'i':
                {
                    helper->execute(mgr, "info", "");
                    prompt_user_to_continue();
                    break;
                }
            case 'r':
                {
                    tg.disable();
                    std::string command;
                    std::cout << "Command to be executed: ";
                    std::getline(std::cin, command);

                    if (command.empty())
                    {
                        break;
                    }
                    int index = 0;
                    for (int i = 0; i < command.size(); i++)
                    {
                        if (command[i] == ' ')
                        {
                            index = i;
                            break;
                        }
                    }
                    std::string cmd = index == 0 ? command : command.substr(0, index);
                    std::string arguments = index == 0 ? "" : command.substr(index + 1);
                    helper->execute(mgr, cmd, arguments);
                    prompt_user_to_continue();
                    tg.enableRaw();
                    break;
                }
            case 's':
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
            walk... there is always on argument

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
