//
// Created by robertvokac on 8/4/25.
//

#include "miniwiki/Command/RmCommand.h"

#include <iostream>

namespace miniwiki::Command {
    void RmCommand::execute(Manager::MiniWikiManager &mgr, const std::string &args) {
        mgr.note_manager.remove_note(args);
    }
    void RmCommand::help() {
        std::cout << "help to be implemented" << std::endl;
    }
}
