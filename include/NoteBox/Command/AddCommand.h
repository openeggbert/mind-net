//
// Created by robertvokac on 8/4/25.
//
#ifndef ADDCOMMAND_H
#define ADDCOMMAND_H

#include "ICommand.h"

namespace NoteBox::Command {
    class AddCommand : public ICommand {
    public:
        void execute(Manager::NoteManager& mgr, const std::string& args) override;
        void help() override;
    };
}

#endif // ADDCOMMAND_H
