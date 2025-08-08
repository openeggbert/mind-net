//
// Created by robertvokac on 8/4/25.
//

#include "NoteBox/Command/LsCommand.h"

#include <iostream>

namespace NoteBox::Command {
    void LsCommand::execute(Manager::NoteBoxManager &mgr, const std::string &args) {
        auto current_path = mgr.note_manager.pwd();
        mgr.note_manager.listNotes(current_path);
    }
    void LsCommand::help() {
        std::cout << "help to be implemented" << std::endl;
    }
}
