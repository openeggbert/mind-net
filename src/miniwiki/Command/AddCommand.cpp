//
// Created by robertvokac on 8/4/25.
//

#include "miniwiki/Command/AddCommand.h"

#include <iostream>

#include "miniwiki/Global.h"

namespace miniwiki::Command {
    void AddCommand::execute(Manager::MiniWikiManager &mgr, const std::string &args)
    {
        std::string title;

        if (args.empty())
        {

            {
                std::cout << "Title: ";
                getline(std::cin,title); std::cin.clear();
                if (title.empty())
                {
                    err << "Title cannot be empty." << std::endl;
                    return;
                }
            }
        }
        std::cout << "Going to add new note with title \"" << (title.empty() ? args : title) << "\"" << std::endl;
        mgr.note_manager.create_note(title.empty() ? args : title);
        if (args.empty() && Utils::ask_yes_no("Do you want to add another note?")) {
            execute(mgr, "");
        }

    }
    void AddCommand::help() {
        std::cout << R"(
NAME
       add

SYNOPSIS
       add [TITLE]

DESCRIPTION
       Creates new Note. The current Note is used as the parent Note ID.

EXAMPLES
       add ... creates new Note, the user will be asked for the title
       add My Projects ... creates new Note with Title "My Projects"
       add #My Projects ... creates new Note with Title "My Projects", but user is not asked for the Question and Content. This is fast mode for the Note creation.

)" << std::endl;
    }
}
