//
// Created by robertvokac on 8/4/25.
//

#include "NoteBox/Command/ShowCommand.h"

#include <iostream>

#include "NoteBox/Global.h"

namespace NoteBox::Command {
    void ShowCommand::execute(Manager::NoteBoxManager &mgr, const std::string &args) {
        std::string path;
        if (!args.empty())
        {
            path = args;
        } else
        {
            auto current_path = mgr.note_manager.pwd();
            if (current_path.empty())
            {
                err << "Cannot get show content of root directory" << std::endl;
                return;
            }
            path = current_path;

        }

        std::cout << "Title: " << mgr.note_manager.read_note(path).title << std::endl;
        std::cout << "ID: " << mgr.note_manager.read_note(path).id << std::endl;
        std::cout << std::string(80, '-') << std::endl;
        auto text = mgr.note_manager.read_content(path).value;
        std::cout << std::endl << std::string(80, '-') << std::endl;
        std::cout << text << std::endl;

    }
    void ShowCommand::help() {
        std::cout << R"(
NAME
       show

SYNOPSIS
       show [NOTE_ID]

DESCRIPTION
       Shows the content of the Note ID.

EXAMPLES
       show ... shows the content of the current Note ID.
       show 3kd43 ... shows the content of the Note with ID 3kd43.

)" << std::endl;
    }
}
