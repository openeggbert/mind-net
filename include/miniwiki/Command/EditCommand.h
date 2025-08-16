//
// Created by robertvokac on 8/4/25.
//
#ifndef EDITCOMMAND_H
#define EDITCOMMAND_H


#include "ICommand.h"

namespace MiniWiki::Command {
    class EditCommand : public ICommand {
    public:
        void execute(Manager::MiniWikiManager& mgr, const std::string& args) override;
        void help() override;
    };
}


#endif // EDITCOMMAND_H
