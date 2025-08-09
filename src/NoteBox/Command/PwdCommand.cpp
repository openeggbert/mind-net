//
// Created by robertvokac on 8/4/25.
//

#include "NoteBox/Command/PwdCommand.h"

#include <iostream>

namespace NoteBox::Command {
    void PwdCommand::execute(Manager::NoteBoxManager &mgr, const std::string &args) {
        std::cout << "/" << mgr.note_manager.pwd() << "\n";
    }
    void PwdCommand::help()     {
        std::cout << R"(
NAME
       pwd

SYNOPSIS
       pwd

DESCRIPTION
       pwd prints the current path, which is the ID of the current Note.

EXAMPLES
       pwd ... it has no argument

)" << std::endl;
    }
}
