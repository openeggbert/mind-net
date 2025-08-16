//
// Created by robertvokac on 8/4/25.
//
#ifndef WALKCOMMAND_H
#define WALKCOMMAND_H


#include "ICommand.h"

namespace miniwiki::Command {
    class WalkCommand : public ICommand {
        ICommandHelper* helper = nullptr;


    public:
        void execute(Manager::MiniWikiManager& mgr, const std::string& args) override;
        void help() override;
        void setCommandHelper(ICommandHelper* factory) override;
        ICommandHelper* get_command_helper() override;
    };
}
#endif // WALKCOMMAND_H
