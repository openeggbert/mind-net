//
// Created by robertvokac on 8/4/25.
//

#include "miniwiki/Command/NameCommand.h"

#include <iostream>

#include "miniwiki/Global.h"

namespace MiniWiki::Command {
    void NameCommand::execute(Manager::MiniWikiManager &mgr, const std::string &args) {
        auto current_path = mgr.note_manager.pwd();
        if (current_path.empty())
        {
            err << "Cannot get name of root directory" << std::endl;
            return;
        }
        std::cout << mgr.note_manager.read_note(current_path).title << "\n";
    }
    void NameCommand::help()     {
        std::cout << R"(
NAME
       name

SYNOPSIS
       name

DESCRIPTION
       name prints the name of the current Note. If the path is /, an error is shown.

EXAMPLES
       name ... it has no argument

)" << std::endl;
    }
}
