//
// Created by robertvokac on 8/4/25.
//
#ifndef UPDATECOMMAND_H
#define UPDATECOMMAND_H


#include "ICommand.h"

namespace MiniWiki::Command {
    class UpdateCommand : public ICommand {
    public:
        void execute(Manager::MiniWikiManager& mgr, const std::string& args) override;
        void help() override;
    };
}

#endif // UPDATECOMMAND_H
