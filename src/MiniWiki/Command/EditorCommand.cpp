//
// Created by robertvokac on 8/4/25.
//

#include "MiniWiki/Command/EditorCommand.h"

#include <iostream>

#include "MiniWiki/Global.h"

namespace MiniWiki::Command {
    void EditorCommand::execute(Manager::MiniWikiManager &mgr, const std::string &args) {
        auto session = mgr.session_manager.get();
        std::cout << "Current editor: " << session.editor_path << std::endl;
        if (Utils::ask_yes_no("Do you want to set a new editor?"))
        {
         std::string new_editor;
            std::cout << "New editor path: ";
            std::cin.clear();
            std::getline(std::cin, new_editor);
            if (new_editor.empty())
            {
                err << "Editor path cannot be empty." << std::endl;
                return;
            }
            session.editor_path = new_editor;
            mgr.session_manager.update(session);
        }
    }
    void EditorCommand::help()     {
        std::cout << R"(
NAME
       editor

SYNOPSIS
       editor

DESCRIPTION
       editor shows the current editor and lets the user sets a new one.

EXAMPLES
       editor ... editor does not have any arguments

)" << std::endl;
    }
}
