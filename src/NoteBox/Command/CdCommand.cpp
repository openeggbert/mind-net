//
// Created by robertvokac on 8/4/25.
//

#include "NoteBox/Command/CdCommand.h"

#include <iostream>

namespace NoteBox::Command {
    void CdCommand::execute(Manager::NoteBoxManager &mgr, const std::string &args) {
        mgr.note_manager.cd(args);
        auto session = mgr.session_manager.get();
        session.current_path = mgr.note_manager.pwd();
        mgr.session_manager.update(session);
        // std::cout << ":" << mgr.note_manager.pwd() << "\n";
    }
    void CdCommand::help() {
        std::cout << R"(
NAME
       cd

SYNOPSIS
       cd  [ID]

DESCRIPTION
       Changes the current Note.

EXAMPLES
       cd ... set the path to the root
       cd /4ad2 ... set the path to the Note with ID 4a2
       cd ad ... set the path to the Note with ID 4ad, if the current Note ID was 4
       cd ... set the path to the parent Note

)" << std::endl;
    }
}
