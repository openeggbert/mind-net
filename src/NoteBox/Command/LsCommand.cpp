//
// Created by robertvokac on 8/4/25.
//

#include "NoteBox/Command/LsCommand.h"

#include <iostream>

namespace NoteBox::Command {
    void LsCommand::execute(Manager::NoteBoxManager &mgr, const std::string &args) {
        mgr.note_manager.listNotes();
    }
    void LsCommand::help() {
        std::cout << "help to be implemented" << std::endl;
    }
}
