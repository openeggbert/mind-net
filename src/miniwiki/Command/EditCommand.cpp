//
// Created by robertvokac on 8/4/25.
//

#include "miniwiki/Command/EditCommand.h"

#include <iostream>

#include "miniwiki/Global.h"
#include "miniwiki/Manager/MiniWikiManager.h"

namespace miniwiki::Command {
    void EditCommand::execute(Manager::MiniWikiManager &mgr, const std::string &args) {
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
        auto content = mgr.note_manager.read_content(path);
        auto text = content.value;
        auto old_text = text;
        content.value = Utils::editTextInEditor(text, mgr.session_manager.get().editor_path);
        if (!Utils::ask_yes_no("Do you want to save the changes?")) {return;}
        mgr.note_manager.update_content(content, old_text);

    }
    void EditCommand::help() {
        std::cout << R"(
NAME
       edit

SYNOPSIS
       edit [NOTE_ID]

DESCRIPTION
       Edits the content of the Note ID.

EXAMPLES
       edit ... edits the content of the current Note ID.
       edit 3kd43 ... edits the content of the Note with ID 3kd43.

)" << std::endl;
    }
}
