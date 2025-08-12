//
// Created by robertvokac on 8/4/25.
//

#include "NoteBox/Command/InfoCommand.h"

#include <iostream>

#include "NoteBox/Global.h"
#include "NoteBox/Enum/Difficulty.h"
#include "NoteBox/Enum/Importance.h"

namespace NoteBox::Command {
    void InfoCommand::execute(Manager::NoteBoxManager &mgr, const std::string &args) {
        auto current_path = mgr.note_manager.pwd();
        if (current_path.empty())
        {
            err << "Cannot get info of root directory" << std::endl;
            return;
        }

        auto note = mgr.note_manager.read_note(current_path);
        std::cout << "ID: " << note.id << "\n"
            << "Parent Note ID: " << note.parent_note_id << "\n"
            << "Title: " << note.title << "\n"
            << "Content ID: " << note.content_id << "\n"
            << "Question: " << note.question << "\n"
            << "Created at: " << Utils::unixToFormattedString(note.created_at) << "\n"
            << "Updated at: " << Utils::unixToFormattedString(note.updated_at) << "\n"
            << "Last shown at: " << Utils::unixToFormattedString(note.last_shown_at) << "\n"
            << "Last reviewed at: " << Utils::unixToFormattedString(note.last_reviewed_at) << "\n"
            << "Expires at: " << Utils::unixToFormattedString(note.expires_at) << "\n"
            << "Review in days: " << (note.review_in_x_days == 0 ? "none" : std::to_string(note.review_in_x_days)) << "\n"
            << "Importance: " << Enum::importance_to_string(note.importance) << "\n"
            << "Difficulty: " << Enum::difficulty_to_string(note.difficulty) << "\n"
            << "Source ID: " << note.source_id << "\n";
    }
    void InfoCommand::help()     {
        std::cout << R"(
NAME
       info

SYNOPSIS
       info

DESCRIPTION
       info prints the information about the current Note. If the path is /, an error is shown.

EXAMPLES
       info ... it has no argument

)" << std::endl;
    }
}
