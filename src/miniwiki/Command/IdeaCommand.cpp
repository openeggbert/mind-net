//
// Created by robertvokac on 8/4/25.
//

#include "miniwiki/Command/IdeaCommand.h"

#include <iostream>

#include "miniwiki/Global.h"

namespace miniwiki::Command
{
#include <string>
#include <ctime>
#include <stdexcept>

    int64_t parse_date_to_unixtime(const std::string& date_str)
    {
        int year, month, day;
        if (sscanf(date_str.c_str(), "%d-%d-%d", &year, &month, &day) != 3) {
            throw std::runtime_error("Failed to parse date string: " + date_str);
        }

        std::tm tm = {};
        tm.tm_year = year - 1900;  // tm_year = years since 1900
        tm.tm_mon  = month - 1;    // tm_mon = 0..11
        tm.tm_mday = day;
        tm.tm_hour = 0;
        tm.tm_min  = 0;
        tm.tm_sec  = 0;

        std::time_t t = std::mktime(&tm);
        if (t == -1) {
            throw std::runtime_error("Failed to convert tm to time_t");
        }

        return static_cast<int64_t>(t);
    }



    void IdeaCommand::execute(Manager::MiniWikiManager& mgr, const std::string& args)
    {
        std::vector<str> arguments = Utils::split_with_quotes(args);

        str arg1 = arguments.empty() ? "add" : arguments[0];
        if (arg1 == "add")
        {
            std::cout << "Enter category (optional): ";
            std::string category;
            std::getline(std::cin, category);

            // std::cout << "Enter due date (optional, YYYY-MM-DD): ";
            std::string due_date;
            // std::getline(std::cin, due_date);

            Entity::Idea idea;
            idea.category = category;
            idea.created_at = Utils::currentUnixTimestamp();
            idea.due_date = due_date.empty() ? -1 : parse_date_to_unixtime(due_date);
            auto text = Utils::editTextInEditor("", mgr.session_manager.get().editor_path);
            if (text.empty())
            {
                err << "Idea text cannot be empty." << std::endl;
                return;
            }
            idea.text = text;
            mgr.idea_manager.create_idea(idea);
            std::cout << idea << std::endl << idea.to_json();


        } else
        {
            err << "Invalid idea command" << std::endl;
            return;
        }

    }

    void IdeaCommand::help() {
        std::cout << R"(
NAME
       idea

SYNOPSIS
       idea
       idea add [CATEGORY]
       idea show
       idea update
       idea rm
       idea ls
DESCRIPTION
       Manipulates with ideas

EXAMPLES
       idea ... create new idea
       idea add ... create new idea

)" << std::endl;
    }
}
