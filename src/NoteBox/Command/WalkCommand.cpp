//
// Created by robertvokac on 8/4/25.
//

#include "NoteBox/Command/WalkCommand.h"

#include <iostream>

#include "NoteBox/Global.h"

namespace NoteBox::Command
{
    void show(Manager::NoteBoxManager& mgr, ICommandHelper* helper)
    {
        helper->execute(mgr, "show", "");
    }

    void WalkCommand::execute(Manager::NoteBoxManager& mgr, const std::string& args)
    {
        Utils::clearScreen();

        show(mgr, helper);

        char choice;
        while (true)
        {
            std::cin >> choice;
            std::cin.clear();

            choice = std::tolower(choice);

            switch (choice)
            {
            case 'e':
                {
                    helper->execute(mgr, "edit", "");
                    break;
                }
            case 'h':
                {
                    std::string command;
                    std::cout << "Argument (empty shows all available commands): ";
                    std::cin >> command;
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
                    break;
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
