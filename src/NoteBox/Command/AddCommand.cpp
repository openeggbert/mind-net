//
// Created by robertvokac on 8/4/25.
//

#include "NoteBox/Command/AddCommand.h"

#include <iostream>

namespace NoteBox::Command {
    void AddCommand::execute(Manager::NoteBoxManager &mgr, const std::string &args) {
        mgr.note_manager.createNote(args);
    }
    void AddCommand::help() {
        std::cout << "help to be implemented" << std::endl;
    }
}
