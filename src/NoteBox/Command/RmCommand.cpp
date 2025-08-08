//
// Created by robertvokac on 8/4/25.
//

#include "NoteBox/Command/RmCommand.h"

#include <iostream>

namespace NoteBox::Command {
    void RmCommand::execute(Manager::NoteBoxManager &mgr, const std::string &args) {
        mgr.note_manager.removeNote(args);
    }
    void RmCommand::help() {
        std::cout << "help to be implemented" << std::endl;
    }
}
