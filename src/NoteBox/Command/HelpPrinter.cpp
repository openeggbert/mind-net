//
// Created by robertvokac on 8/4/25.
//

#include "NoteBox/Command/HelpPrinter.h"

#include <iostream>

#include "NoteBox/Global.h"

namespace NoteBox::Command
{
    HelpPrinter::HelpPrinter(NoteBox::Command::CommandFactory* factory_)
    {
        this->factory = factory_;
    }

    HelpPrinter::~HelpPrinter() = default;

    void HelpPrinter::print(const std::string& cmd)
    {
        if (this->factory == nullptr)
        {
            throw std::runtime_error("factory is not set");
        }
        auto command = factory->getCommand(cmd);
        if (command)
        {
            command->help();
        }
        else
        {
            err << "Unknown command: " << cmd << "\n";
        }
    }

    std::vector<std::string> HelpPrinter::list_commands()
    {
        return this->factory->list_commands();
    }
}
