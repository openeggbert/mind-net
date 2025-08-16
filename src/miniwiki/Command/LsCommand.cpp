//
// Created by robertvokac on 8/4/25.
//

#include "miniwiki/Command/LsCommand.h"

#include <iostream>

namespace MiniWiki::Command {
    void LsCommand::execute(Manager::MiniWikiManager &mgr, const std::string &args) {
        auto current_path = mgr.note_manager.pwd();
        mgr.note_manager.list_notes(current_path);
    }
    void LsCommand::help() {
        std::cout << "help to be implemented" << std::endl;
    }
}
