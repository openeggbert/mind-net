//
// Created by robertvokac on 8/4/25.
//
#ifndef HELPCOMMAND_H
#define HELPCOMMAND_H

#include "ICommand.h"

namespace miniwiki::Command {
    class HelpCommand : public ICommand {
    private:
        ICommandHelper* command_helper = nullptr;
    public:
        void execute(Manager::MiniWikiManager& mgr, const std::string& args) override;
        void help() override;

        void setCommandHelper(ICommandHelper* factory) override;
        ICommandHelper* get_command_helper() override;
    };
}

#endif // HELPCOMMAND_H
