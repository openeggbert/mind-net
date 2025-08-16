//
// Created by robertvokac on 8/4/25.
//
#ifndef LITCOMMAND_H
#define LITCOMMAND_H

#include "ICommand.h"
#include "MiniWiki/Manager/LiteratureSourceManager.h"

namespace MiniWiki::Command {
    class LitCommand : public ICommand {
    public:
        void execute(Manager::MiniWikiManager& mgr, const std::string& args) override;
        void help() override;
    };
}
#endif // LITCOMMAND_H
