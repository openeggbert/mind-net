//
// Created by robertvokac on 8/4/25.
//

#include "miniwiki/Command/TermCommand.h"
#include <iostream>
#include "miniwiki/Global.h"
#include <string>
#include <vector>
#include <sstream>

namespace MiniWiki::Command
{

    std::vector<std::string> splitByLines(const std::string& text) {
        std::istringstream iss(text);
        std::string line;
        std::vector<std::string> lines;

        while (std::getline(iss, line)) {
            // Handle Windows-style "\r\n"
            if (!line.empty() && line.back() == '\r') {
                line.pop_back();
            }
            lines.push_back(line);
        }

        return lines;
    }


    void TermCommand::execute(Manager::MiniWikiManager& mgr, const std::string& args)
    {
        std::string used_path;
        auto pwd = mgr.note_manager.pwd();

        if (pwd.empty())
        {
            used_path = args;
        }
        else
        {
            if (Utils::get_note_id_from_args(pwd, args, used_path)) return;
        }
        if (args.empty())
        {
            err << "term command requires an argument" << std::endl;
            return;
        }
        std::vector<str> arguments = Utils::split_with_quotes(args);

        const auto& argument1 = arguments[0];
        auto argument2 = arguments.size() > 1 ? arguments[1] : "";
        auto argument3 = arguments.size() > 2 ? arguments[2] : "";
        if (argument1 == "ls")
        {
            // if (argument2.empty())
            // {
            //     err << "term ls command requires a category as the argument" << std::endl;
            //     return;
            // }
            int page_size = 50;
            int page_number = 0;
            while (true)
            {
                auto terms = helper->getDB()->get()->term_repository->list(argument2, page_number++, page_size);
                if (terms.empty())
                {
                    break;
                }
                std::cout << "ID | Category | Name | Note ID" << std::endl;
                for (auto t : terms)
                {
                    std::cout << t.id << " | " << t.category << " | " << t.name << " | " << t.note_id << std::endl;
                }
            }
        }
        else if (argument1 == "add")
        {
            if (arguments.size() < 3)
            {
                //err << "term add command requires a name and category as the argument" << std::endl;
                std::string category;

                std::cout << "Category : ";
                std::getline(std::cin, category);
                if (category.empty())
                {
                 err << "Category cannot be empty" << std::endl;
                    return;
                }
                std::string terms =
                    Utils::editTextInEditor("#Please, add new terms. Each term for one line.\n\n\n\n\n",
                        mgr.session_manager.get().editor_path);

                std::vector<str> lines = splitByLines(terms);
                for (auto l : lines)
                {
                    if (l.empty())
                    {
                        continue;
                    }
                    if (l[0] == '#')
                    {
                        continue;
                    }

                    Entity::Term term;
                    term.name = l;
                    term.category = category;
                    try
                    {
                        helper->getDB()->get()->term_repository->create(term);
                    } catch (std::exception& e)
                    {
                        err << "name and category must be unique" << std::endl;
                        return;
                    }
                }
                return;
            }
            const str& category = argument2;
            const str& name = argument3;
            if (name.empty() || category.empty())
            {
                err << "term add command requires a name and category as the argument" << std::endl;
                return;
            }
            Entity::Term term;
            term.name = name;
            term.category = category;
            try
            {
                helper->getDB()->get()->term_repository->create(term);
            } catch (std::exception& e)
            {
                err << "name and category must be unique" << std::endl;
                return;
            }
        }
        else if (argument1 == "rm")
        {
            if (arguments.size() < 3)
            {
                err << "term rm command requires a name and category as the argument" << std::endl;
                return;
            }
            str& category = argument2;
            str& name = argument3;
            if (name.empty() || category.empty())
            {
                err << "term add command requires a name and category as the argument" << std::endl;
                return;
            }
            Entity::Term term;
            str term_id;
            try
            {
                term_id = helper->getDB()->get()->term_repository->id_for_name_and_category(name, category);
            } catch (std::exception& e)
            {
                err << "term rm command requires a name and category as the argument" << std::endl;
                return;
            }
            helper->getDB()->get()->term_repository->remove(term_id);
        }
        else if (argument1 == "edit")
        {
            if (arguments.size() < 3)
            {
                err << "term rm command requires a name and category as the argument" << std::endl;
                return;
            }
            str& category = argument2;
            str& name = argument3;
            if (name.empty() || category.empty())
            {
                err << "term add command requires a name and category as the argument" << std::endl;
                return;
            }

            str term_id;
            try
            {
                term_id = helper->getDB()->get()->term_repository->id_for_name_and_category(name, category);
            } catch (std::exception& e)
            {
                err << "term rm command requires a name and category as the argument" << std::endl;
                return;
            }

            Entity::Term term = helper->getDB()->get()->term_repository->read(term_id);
            std::cout << R"(
What field do you want to update?
0 exit
1 name
2 category
3 note_id

)";

            char choice;
            std::cin >> choice;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            switch (choice)
            {
            case '0': return;
            case '1':
                {
                    std::string text;

                    std::cout << "Name: ";
                    std::getline(std::cin, text);
                    term.name = text;
                }
                break;

            case '2':
                {
                    std::string text;

                    std::cout << "Category: ";
                    std::getline(std::cin, text);
                    term.category = text;
                }
                break;


            case '3':
                {
                    std::string text;

                    std::cout << "Note ID: ";
                    std::getline(std::cin, text);
                    if (!helper->getDB()->get()->note_repository->does_id_exist(text))
                    {
                        err << "Note with id " << text << " does not exist" << std::endl;
                        return;
                    }
                    term.note_id = text;
                }
                break;

            default: err << "Invalid choice" << std::endl;
                return;
            }
            helper->getDB()->get()->term_repository->update(term);
        }
        else if (argument1 == "cat")
        {
            for (const auto& c : helper->getDB()->get()->term_repository->list_categories())
            {
                std::cout << c << std::endl;
            }
        }
        else
            err << "term command " << argument1 << " not implemented" << std::endl;
    }

    void TermCommand::help()
    {
        std::cout << "help to be implemented" << std::endl;
    }

    void TermCommand::setCommandHelper(ICommandHelper* command_helper_)
    {
        helper = command_helper_;
    };

    ICommandHelper* TermCommand::get_command_helper()
    {
        return helper;
    }
}
