//
// Created by robertvokac on 8/4/25.
//
#ifndef IDEACOMMAND_H
#define IDEACOMMAND_H


#include "ICommand.h"

namespace NoteBox::Command {
    class IdeaCommand : public ICommand {
    public:
        void execute(Manager::NoteBoxManager& mgr, const std::string& args) override;
        void help() override;
    };
}

#endif // IDEACOMMAND_H
