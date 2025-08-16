//
// Created by robertvokac on 8/4/25.
//
#ifndef INFOCOMMAND_H
#define INFOCOMMAND_H


#include "ICommand.h"

namespace MiniWiki::Command {
    class InfoCommand : public ICommand {
    public:
        void execute(Manager::MiniWikiManager& mgr, const std::string& args) override;
        void help() override;
    };
}

#endif // INFOCOMMAND_H
