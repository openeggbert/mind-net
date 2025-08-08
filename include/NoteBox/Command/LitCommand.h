//
// Created by robertvokac on 8/4/25.
//
#ifndef LITCOMMAND_H
#define LITCOMMAND_H

#include "ICommand.h"
#include "NoteBox/Manager/LiteratureSourceManager.h"

namespace NoteBox::Command {
    class LitCommand : public ICommand {
    public:
        void execute(Manager::NoteBoxManager& mgr, const std::string& args) override;
        void help() override;
    };
}
#endif // LITCOMMAND_H
