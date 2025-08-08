//
// Created by robertvokac on 8/4/25.
//
#ifndef HELPCOMMAND_H
#define HELPCOMMAND_H

#include "ICommand.h"

namespace NoteBox::Command {
    class HelpCommand : public ICommand {
    private:
        IHelpPrinter* help_printer = nullptr;
    public:
        void execute(Manager::NoteBoxManager& mgr, const std::string& args) override;
        void help() override;

        void setHelpPrinter(IHelpPrinter* factory) override;
        IHelpPrinter* getHelpPrinter() override;
    };
}

#endif // HELPCOMMAND_H
