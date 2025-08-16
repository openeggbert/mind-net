//
// Created by robertvokac on 8/4/25.
//
#ifndef UPTIMECOMMAND_H
#define UPTIMECOMMAND_H


#include "ICommand.h"

namespace MiniWiki::Command {
    class UptimeCommand : public ICommand {
    public:
        void execute(Manager::MiniWikiManager& mgr, const std::string& args) override;
        void help() override;
    };
}


#endif // UPTIMECOMMAND_H
