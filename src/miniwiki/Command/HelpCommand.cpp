//
// Created by robertvokac on 8/4/25.
//

#include "miniwiki/Command/HelpCommand.h"

#include <iostream>

namespace MiniWiki::Command
{
    void HelpCommand::execute(Manager::MiniWikiManager& mgr, const std::string& args)
    {
        //std::cout << "help command has argument \"" << args << "\"" << std::endl;

        if (this->command_helper == nullptr)
        {
            throw std::runtime_error("help_printer is not set");
        }

        if (args.empty())
        {
            std::cout << "Available commands:" << std::endl << std::endl;
            for (const std::string& cmd : this->command_helper->list_commands())
            {
                std::cout << cmd << std::endl;
            }
            return;
        }

        command_helper->print_help(args);
    }

    void HelpCommand::help()
    {
        std::cout << R"(
NAME
       help

SYNOPSIS
       help [command]

DESCRIPTION
       help is the user's manual. The argument is the command, user wants to get the information about.

EXAMPLES
       help ... help without argument shows the list of all available commands
       help ls
       help help
       help show

)" << std::endl;
    }

    void HelpCommand::setCommandHelper(ICommandHelper* command_helper_)
    {
        command_helper = command_helper_;
    };

    ICommandHelper* HelpCommand::get_command_helper()
    {
        return command_helper;
    }
}
