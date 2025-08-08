//
// Created by robertvokac on 8/4/25.
//
#ifndef CDCOMMAND_H
#define CDCOMMAND_H

#include "ICommand.h"

namespace NoteBox::Command {
    class CdCommand : public ICommand {
    public:
        void execute(Manager::NoteBoxManager& mgr, const std::string& args) override;
        void help() override;
    };
}
#endif // CDCOMMAND_H
