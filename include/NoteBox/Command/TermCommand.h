//
// Created by robertvokac on 8/4/25.
//
#ifndef TERMCOMMAND_H
#define TERMCOMMAND_H

#include "ICommand.h"

namespace NoteBox::Command {
    class TermCommand : public ICommand {

        ICommandHelper* helper = nullptr;

    public:
        void execute(Manager::NoteBoxManager& mgr, const std::string& args) override;
        void help() override;
        void setCommandHelper(ICommandHelper* factory) override;
        ICommandHelper* get_command_helper() override;
    };
}
#endif // TERMCOMMAND_H
