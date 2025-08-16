//
// Created by robertvokac on 8/4/25.
//
#ifndef NAMECOMMAND_H
#define NAMECOMMAND_H


#include "ICommand.h"

namespace miniwiki::Command {
    class NameCommand : public ICommand {
    public:
        void execute(Manager::MiniWikiManager& mgr, const std::string& args) override;
        void help() override;
    };
}

#endif // NAMECOMMAND_H
