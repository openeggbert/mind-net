//
// Created by robertvokac on 8/4/25.
//

#include "NoteBox/Command/HelpCommand.h"

#include <iostream>

namespace NoteBox::Command
{
    void HelpCommand::execute(Manager::NoteBoxManager& mgr, const std::string& args)
    {
        //std::cout << "help command has argument \"" << args << "\"" << std::endl;

        if (args.empty())
        {
            help();
            return;
        }
        if (this->help_printer == nullptr)
        {
            throw std::runtime_error("help_printer is not set");
        }
        help_printer->print(args);

    }

    void HelpCommand::help()
    {
        std::cout << R"(
NAME
       help

SYNOPSIS
       help [command]

DESCRIPTION
       help is the user's manual. The argument is the command, user wants to get the information about

EXAMPLES
       help ls
       help help
       help show

)" << std::endl;
    }

    void HelpCommand::setHelpPrinter(IHelpPrinter* helpPrinter_)
    {
        help_printer = helpPrinter_;
    };

    IHelpPrinter* HelpCommand::getHelpPrinter()
    {
        return help_printer;
    }
}
