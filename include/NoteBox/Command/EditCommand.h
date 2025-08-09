//
// Created by robertvokac on 8/4/25.
//
#ifndef EDITCOMMAND_H
#define EDITCOMMAND_H


#include "ICommand.h"

namespace NoteBox::Command {
    class EditCommand : public ICommand {
    public:
        void execute(Manager::NoteBoxManager& mgr, const std::string& args) override;
        void help() override;
    };
}


#endif // EDITCOMMAND_H
