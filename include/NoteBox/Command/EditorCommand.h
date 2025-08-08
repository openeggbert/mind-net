//
// Created by robertvokac on 8/4/25.
//
#ifndef EDITORCOMMAND_H
#define EDITORCOMMAND_H

#include "ICommand.h"

namespace NoteBox::Command {
    class EditorCommand : public ICommand {
    public:
        void execute(Manager::NoteBoxManager& mgr, const std::string& args) override;
        void help() override;
    };
}
#endif // EDITORCOMMAND_H
