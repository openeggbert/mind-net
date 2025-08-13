//
// Created by robertvokac on 8/4/25.
//

#include "NoteBox/Command/TermCommand.h"

#include <iostream>

#include "NoteBox/Global.h"

namespace NoteBox::Command
{
    Entity::Term validate_and_extract_term_details(str argument2)
    {
        if (argument2.empty())
        {
            err << "term add command requires a name as the argument" << std::endl;
        }
        str name;
        str category;
        bool name_is_finished = false;
        for (int i = 0; i < argument2.size(); i++)
        {
            char ch = argument2[i];
            if (ch == '|')
            {
                name = argument2.substr(0, i);
                category = argument2.substr(i + 1, argument2.size() - i - 1);
                name_is_finished = true;
                break;
            }
        }
        if (!name_is_finished)
        {
            err << "term add command requires a name as the argument" << std::endl;
        }
        Entity::Term term;
        term.name = name;
        term.category = category;
        return term;
    }

    void TermCommand::execute(Manager::NoteBoxManager& mgr, const std::string& args)
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
        str argument;

        for (char ch : args)
        {
            if (ch == ' ')
            {
                break;
            }
            argument += ch;
        }
        str argument2;
        if (argument.size() < args.size())
        {
            argument2 = args.substr(argument.size() + 1, args.size() - argument.size() - 1);
        }
        if (argument == "ls")
        {
            if (argument2.empty())
            {
                err << "term ls command requires a category as the argument" << std::endl;
                return;
            }
            int page_size = 50;
            int page_number = 0;
            while (true)
            {
                auto terms = helper->getDB()->get()->term_repository->list(argument2, page_number++, page_size);
                if (terms.empty())
                {
                    break;
                }
                for (auto t : terms)
                {
                    std::cout << t.id << " | " << t.name << " | " << t.category << " | " << t.note_id << std::endl;
                }
            }
        }
        else if (argument == "add")
        {
            Entity::Term term = validate_and_extract_term_details(argument2);
            if (term.name.empty() || term.category.empty())
            {
                err << "term add command requires a name and category as the argument" << std::endl;
                return;
            }
            helper->getDB()->get()->term_repository->create(term);
        }
        else if (argument == "edit")
        {
            Entity::Term term = validate_and_extract_term_details(argument2);
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
            case 0: return;
            case 1:
                {
                    std::string text;

                    std::cout << "Name: ";
                    std::getline(std::cin, text);
                    term.name = text;
                }
                break;

            case 2:
                {
                    std::string text;

                    std::cout << "Category: ";
                    std::getline(std::cin, text);
                    term.category = text;
                }
                break;


            case 3:
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
        else if (argument == "show")
        {
            throw std::runtime_error("term show command not implemented");
            // if (argument2.empty())
            // {
            //     err << "term ls command requires a category as the argument" << std::endl;
            //     return;
            // }
            //
            //
            // auto term_id = helper->getDB()->get()->term_repository->id_for_name_and_category(term.name, category);
            //     auto term = helper->getDB()->get()->term_repository->read(argument2, );
            //     if (terms.empty())
            //     {
            //         break;
            //     }
            //     for (auto t : terms)
            //     {
            //         std::cout << t.id << " | " << t.name << " | " << t.category << " | " << t.note_id << std::endl;
            //     }
            // }
        }
        else if (argument == "cat")
        {
            for (auto c:helper->getDB()->get()->term_repository->list_categories())
            {
                std::cout << c << std::endl;
            }
        } else
            err << "term command " << argument << " not implemented" << std::endl;


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
