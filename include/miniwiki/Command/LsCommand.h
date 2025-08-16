//
// Created by robertvokac on 8/4/25.
//
#ifndef LSCOMMAND_H
#define LSCOMMAND_H

#include "ICommand.h"

namespace miniwiki::Command {
    class LsCommand : public ICommand {
    public:
        void execute(Manager::MiniWikiManager& mgr, const std::string& args) override;
        void help() override;
    };
}

#endif // LSCOMMAND_H
