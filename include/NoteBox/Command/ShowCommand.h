//
// Created by robertvokac on 8/4/25.
//
#ifndef SHOWCOMMAND_H
#define SHOWCOMMAND_H


#include "ICommand.h"

namespace NoteBox::Command {
    class ShowCommand : public ICommand {
    public:
        void execute(Manager::NoteBoxManager& mgr, const std::string& args) override;
        void help() override;
    };
}

#endif // SHOWCOMMAND_H
