//
// Created by robertvokac on 8/4/25.
//

#ifndef VERSIONCOMMAND_H
#define VERSIONCOMMAND_H
#include "ICommand.h"

namespace miniwiki::Command {
    class VersionCommand : public ICommand {
    public:
        void execute(Manager::MiniWikiManager& mgr, const std::string& args) override;
        void help() override;
    };
}

#endif //VERSIONCOMMAND_H
