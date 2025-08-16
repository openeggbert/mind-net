//
// Created by robertvokac on 8/4/25.
//
#ifndef TREECOMMAND_H
#define TREECOMMAND_H

#include "ICommand.h"

namespace MiniWiki::Command {
    class TreeCommand : public ICommand {

        ICommandHelper* helper = nullptr;

    public:
        void execute(Manager::MiniWikiManager& mgr, const std::string& args) override;
        void help() override;
        void setCommandHelper(ICommandHelper* factory) override;
        ICommandHelper* get_command_helper() override;
    };
}
#endif // TREECOMMAND_H
