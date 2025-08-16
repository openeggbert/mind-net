//
// Created by robertvokac on 8/4/25.
//
#ifndef PWDCOMMAND_H
#define PWDCOMMAND_H

#include "ICommand.h"

namespace miniwiki::Command {
    class PwdCommand : public ICommand {
    public:
        void execute(Manager::MiniWikiManager& mgr, const std::string& args) override;
        void help() override;
    };
}

#endif // PWDCOMMAND_H
