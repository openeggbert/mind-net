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
#include <limits>


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

    void print_title(const char title[])
    {
        //std::cout << title << std::endl << std::string("-", 80) << std::endl;
    }

    void prompt_user_to_continue()
    {
        std::cout << "Press any key to continue." << std::endl;
        std::cin.get();
    }

    void show(Manager::NoteBoxManager& mgr, ICommandHelper* helper)
    {
        helper->execute(mgr, "show", "");
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
                    print_title("Edit content");
                    helper->execute(mgr, "edit", "");

                    tg.enableRaw();
                    break;
                }
            case 'g':
                {
                    int page_size = 10;
                    int page_number = 0;
                    bool exit = false;

                    while (!exit)
                    {
                        Utils::clearScreen();
                        auto current_path = mgr.note_manager.pwd();

                        tg.disable();
                        std::cout << "Current path: " << current_path << std::endl;
                        std::cout << std::string(80, '-') << std::endl;
                        tg.enableRaw();

                        auto list = helper->getDB()->get()->note_repository->list(current_path, page_number, page_size);

                        char option = 'a';
                        if (!current_path.empty()) {std::cout << "z | .." << std::endl;}
                        for (const auto& e : list)
                        {
                            std::cout << option << " | " << e.title << " " << e.id << std::endl;
                            option++;
                        }

                        unsigned char c;
                        if (readByte(c) <= 0) continue;
                        char choice = static_cast<char>(std::tolower(c));
                        char end_letter = 'a' + list.size() - 1;

                        if (choice >= 'a' && choice <= end_letter)
                        {
                            helper->execute(mgr, "cd", "/" + list[choice - 'a'].id);
                        }
                        else if (choice == 'm')
                        {
                            if (page_number > 0) page_number--;
                        }
                        else if (choice == 'n')
                        {
                            page_number++;
                        }
                        else if (choice == 'p')
                        {
                            tg.disable();
                            std::cout << "New page number: ";
                            std::cin >> page_number;

                            if (std::cin.fail())
                            {
                                std::cin.clear();
                                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                                std::cout << "Invalid input. Try again." << std::endl;
                                page_number = 0;
                            }
                            tg.enableRaw();
                        }
                        else if (choice == 'q')
                        {
                            tg.disable();
                            std::cout << R"(
a-z ... go to a Note from the list
m ... previous page
n ... next page
p ... go to specific page
q ... show this help
x ... exit Go mode
z ... go to parent directory
)" << std::endl;
                            prompt_user_to_continue();
                            tg.enableRaw();
                        }
                        else if (choice == 'x')
                        {
                            exit = true;
                        }
                        else if (choice == 'z' && !current_path.empty())
                        {
                            helper->execute(mgr, "cd", "..");
                        }
                        else
                        {
                            err << "Unknown command: " << choice << std::endl;
                        }
                    }

                    break;
                }

            case 'h':
                {
                    tg.disable();

                    print_title("Help");
                    std::cout << R"(
e ... edit content
g ... go to a Note from the list
h ... show this help
i ... show info about current Note
r ... run a command
s ... show content of the current Note
x ... exit the Walking mode

)";
                    prompt_user_to_continue();
                    tg.enableRaw();
                    break;
                }
            case 'i':
                {
                    helper->execute(mgr, "info", "");
                    tg.disable();

                    print_title("Show information");
                    prompt_user_to_continue();
                    tg.enableRaw();
                    break;
                }
            case 'r':
                {
                    tg.disable();
                    print_title("Run command");
                    std::string command;

                    std::cout << "How to return to Walking mode: type exit" << std::endl << std::endl;

                    while (true)
                    {
                        std::cout << "Command: ";
                        std::getline(std::cin, command);

                        if (command.empty() || command == "exit")
                        {
                            break;
                        }
                        if (command == "walk")
                        {
                            std::cout << "You can't run walk command, because you are already walking" << std::endl;
                            continue;
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

                        //prompt_user_to_continue();
                    }


                    tg.enableRaw();
                    break;
                }
            case 's':
                {
                    print_title("Show content");
                    helper->execute(mgr, "show", "");
                    break;
                }
            case 'x':
                {
                    print_title("Exit \"Walking mode\"");
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
