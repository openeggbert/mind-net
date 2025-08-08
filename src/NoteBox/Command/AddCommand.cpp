//
// Created by robertvokac on 8/4/25.
//

#include "NoteBox/Command/AddCommand.h"

#include <iostream>

#include "NoteBox/Global.h"

namespace NoteBox::Command {
    void AddCommand::execute(Manager::NoteBoxManager &mgr, const std::string &args) {
        if (args.empty())
        {
            err << "add command requires an argument" << std::endl;
            return;
        }
        std::cout << "Going to add new note with title \"" << args << "\"" << std::endl;
        mgr.note_manager.createNote(args);
    }
    void AddCommand::help() {
        std::cout << "help to be implemented" << std::endl;
    }
}
