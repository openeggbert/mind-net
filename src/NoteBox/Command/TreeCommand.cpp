//
// Created by robertvokac on 8/4/25.
//

#include "NoteBox/Command/TreeCommand.h"

#include <iostream>

namespace NoteBox::Command
{
    using std::string;
    typedef string str;

    void print_line_of_tree(str& note_id, const str& note_title, const std::string& prefix, bool is_last, Persistence::Api::NoteRepository*& note_repo)
    {
        if (!note_id.empty())
        {
            std::cout << prefix;
            std::cout << (is_last ? "└── " : "├── ");
            std::cout << note_id << " - " << note_title << std::endl;
        }

        int page_number = 0;
        int page_size = 50;
        std::vector<Entity::Note> children;

        while (true)
        {
            auto tmp = note_repo->list(note_id, page_number++, page_size);
            if (tmp.empty()) break;
            children.insert(children.end(), tmp.begin(), tmp.end());
        }

        for (size_t i = 0; i < children.size(); ++i)
        {
            bool last = (i == children.size() - 1);
            std::string new_prefix = prefix + (is_last ? "    " : "│   ");
            print_line_of_tree(children[i].id, children[i].title, new_prefix, last, note_repo);
        }
    }

    void TreeCommand::execute(Manager::NoteBoxManager& mgr, const std::string& args)
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

        std::shared_ptr<Persistence::DB>* db = get_command_helper()->getDB();

        str note_id;
        str note_title;
        if (!used_path.empty())
        {
            auto note = mgr.note_manager.read_note(used_path);
            note_id = note.id;
            note_title = note.title;
        }

        auto note_repo = db->get()->note_repository;
        print_line_of_tree(note_id, note_title, "" , true, note_repo);
    }

    void TreeCommand::help()
    {
        std::cout << "help to be implemented" << std::endl;
    }

    void TreeCommand::setCommandHelper(ICommandHelper* command_helper_)
    {
        helper = command_helper_;
    };

    ICommandHelper* TreeCommand::get_command_helper()
    {
        return helper;
    }
}
