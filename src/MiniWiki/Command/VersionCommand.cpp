//
// Created by robertvokac on 8/4/25.
//

#include "MiniWiki/Command/VersionCommand.h"

#include <iostream>

namespace MiniWiki::Command {
    void VersionCommand::execute(Manager::MiniWikiManager &mgr, const std::string &args) {
        std::cout << "0.0.0-SNAPSHOT" << std::endl;
    }
    void VersionCommand::help() {
        std::cout << "help to be implemented" << std::endl;
    }
}
