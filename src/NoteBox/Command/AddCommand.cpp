//
// Created by robertvokac on 8/4/25.
//

#include "NoteBox/Command/AddCommand.h"

#include <iostream>

namespace NoteBox::Command {
    void AddCommand::execute(Manager::NoteManager &mgr, const std::string &args) {
        mgr.addNote(args);
    }
    void AddCommand::help() {
        std::cout << "help to be implemented" << std::endl;
    }
}
