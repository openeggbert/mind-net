//
// Created by robertvokac on 8/4/25.
//
#ifndef RMCOMMAND_H
#define RMCOMMAND_H

#include "ICommand.h"

namespace MiniWiki::Command {
    class RmCommand : public ICommand {
    public:
        void execute(Manager::MiniWikiManager& mgr, const std::string& args) override;
        void help() override;
    };
}

#endif // RMCOMMAND_H
