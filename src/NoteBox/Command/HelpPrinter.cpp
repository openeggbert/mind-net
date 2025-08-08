//
// Created by robertvokac on 8/4/25.
//

#include "NoteBox/Command/HelpPrinter.h"

#include <iostream>

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
        if (command) {
            command->help();
        } else {
            std::cerr << "Unknown command: " << cmd << "\n";
        }
    }
}
