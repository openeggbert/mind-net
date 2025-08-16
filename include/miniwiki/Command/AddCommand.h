//
// Created by robertvokac on 8/4/25.
//
#ifndef ADDCOMMAND_H
#define ADDCOMMAND_H

#include "ICommand.h"

namespace miniwiki::Command {
    class AddCommand : public ICommand {
    public:
        void execute(Manager::MiniWikiManager& mgr, const std::string& args) override;
        void help() override;
    };
}

#endif // ADDCOMMAND_H
