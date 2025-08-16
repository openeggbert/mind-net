//
// Created by robertvokac on 8/4/25.
//
#ifndef EDITORCOMMAND_H
#define EDITORCOMMAND_H

#include "ICommand.h"

namespace miniwiki::Command {
    class EditorCommand : public ICommand {
    public:
        void execute(Manager::MiniWikiManager& mgr, const std::string& args) override;
        void help() override;
    };
}
#endif // EDITORCOMMAND_H
