//
// Created by robertvokac on 8/4/25.
//

#include "NoteBox/Command/CdCommand.h"

#include <iostream>

namespace NoteBox::Command {
    void CdCommand::execute(Manager::NoteBoxManager &mgr, const std::string &args) {
        mgr.note_manager.cd(args);
        // std::cout << ":" << mgr.note_manager.pwd() << "\n";
    }
    void CdCommand::help() {
        std::cout << "help to be implemented" << std::endl;
    }
}
