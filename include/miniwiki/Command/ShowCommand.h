//
// Created by robertvokac on 8/4/25.
//
#ifndef SHOWCOMMAND_H
#define SHOWCOMMAND_H


#include "ICommand.h"

namespace miniwiki::Command {
    class ShowCommand : public ICommand {
    public:
        void execute(Manager::MiniWikiManager& mgr, const std::string& args) override;
        void help() override;
    };
}

#endif // SHOWCOMMAND_H
