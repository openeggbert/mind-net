//
// Created by robertvokac on 8/4/25.
//
#ifndef LSCOMMAND_H
#define LSCOMMAND_H

#include "ICommand.h"

namespace NoteBox::Command {
    class LsCommand : public ICommand {
    public:
        void execute(Manager::NoteManager& mgr, const std::string& args) override;
    };
}

#endif // LSCOMMAND_H
