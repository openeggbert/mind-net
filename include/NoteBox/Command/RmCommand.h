//
// Created by robertvokac on 8/4/25.
//
#ifndef RMCOMMAND_H
#define RMCOMMAND_H

#include "ICommand.h"

namespace NoteBox::Command {
    class RmCommand : public ICommand {
    public:
        void execute(Manager::NoteManager& mgr, const std::string& args) override;
    };
}

#endif // RMCOMMAND_H
