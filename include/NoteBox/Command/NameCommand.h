//
// Created by robertvokac on 8/4/25.
//
#ifndef NAMECOMMAND_H
#define NAMECOMMAND_H


#include "ICommand.h"

namespace NoteBox::Command {
    class NameCommand : public ICommand {
    public:
        void execute(Manager::NoteBoxManager& mgr, const std::string& args) override;
        void help() override;
    };
}

#endif // NAMECOMMAND_H
