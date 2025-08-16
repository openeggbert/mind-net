// Created by robertvokac on 8/4/25.

#include "miniwiki/Manager/NoteManager.h"

#include <iomanip>
#include <iostream>

#include "miniwiki/Global.h"
#include "miniwiki/RedCout.h"
#include "miniwiki/Utils.h"
#include "miniwiki/Enum/Importance.h"
#include "miniwiki/Enum/Difficulty.h"
#include "miniwiki/persistence/DB.h"

namespace MiniWiki::Manager
{
    NoteManager::NoteManager(const std::shared_ptr<Persistence::DB>& db)
        : currentPath(""), db(db)
    {
    }

    const std::string& NoteManager::pwd() const
    {
        auto y = db->note_repository->find_youngest_child_note_id(currentPath);
        // std::cout << "Youngest child: " << (y.has_value() ? y.value() : "none") << std::endl;
        return currentPath;
    }

    void NoteManager::cd(const std::string& path)
    {

        std::string new_path = path;
        if (new_path.empty())
        {
            currentPath = "";
            return;
        }
        if (new_path == "..")
        {
            auto parts = Utils::note_id_to_vector(currentPath);
            parts.pop_back();
            currentPath = Utils::vector_to_note_id(parts);
            return;
        }
        if (new_path[0] == '/')
        {
            new_path = new_path.substr(1, new_path.size() - 1);
        } else
        {
            new_path = currentPath + path;
        }

        if (!db->note_repository->does_id_exist(new_path))
        {
            err << "Note with id " << new_path << " does not exist" << std::endl;
            return;
        }
        currentPath = new_path;
    }

    void NoteManager::create_note(const std::string& title)
    {
        if (title.empty()) {throw std::runtime_error("title cannot be empty");}
        bool fast_adding = title[0] == '#';
        std::string updated_title = title;
        if (fast_adding)
        {
            updated_title = updated_title.substr(1, title.size() - 1);
        }
        Entity::Note note;
        using std::cout;
        using std::cin;
        using std::endl;
        typedef std::string s;


        // s current_path = pwd();
        // auto vector_path = current_path.empty() ? std::vector<std::string>{} : Utils::note_id_to_vector(current_path);
        // if (!vector_path.empty()) { vector_path.pop_back(); }
        // auto parent_note_id = Utils::vector_to_note_id(vector_path);
        //
        Entity::Content content;
        s question;
        if (!fast_adding)
        {
            cout << "Question: ";
            getline(cin,question); cin.clear();
        }


        //


        auto parent_note_id = currentPath;
        auto youngest_child_id = db->note_repository->find_youngest_child_note_id(parent_note_id);
        std::string next_id = Utils::next_note_id(parent_note_id, youngest_child_id.value_or(""));
        note.id = next_id;
        note.parent_note_id = parent_note_id;
        note.title = updated_title;
        //
        content.id = note.id;
        content.value = fast_adding ? "" : Utils::editTextInEditor("# " + note.title + "\n\n", db->session_repository->get().editor_path);
        note.content_id = content.id;
        db->content_repository->create(content);
        //
        note.question = question;
        note.created_at = Utils::currentUnixTimestamp();
        note.updated_at = note.created_at;
        note.last_shown_at = 0l;
        note.last_reviewed_at = 0l;
        note.expires_at = 0l;
        note.review_in_x_days = 0;
        note.importance = static_cast<int>(MiniWiki::Enum::Importance::MEDIUM);
        note.difficulty = static_cast<int>(MiniWiki::Enum::Difficulty::MEDIUM);
        if (!fast_adding && !MiniWiki::Utils::ask_yes_no("Do you really want to create this new Note?")) {return;}
        db->note_repository->create(note);
    }

    Entity::Note NoteManager::read_note(std::string& id)
    {
        return db->note_repository->read(id);
    }

    void NoteManager::update_note(Entity::Note& note)
    {
        db->note_repository->update(note);
    }

    void NoteManager::remove_note(const std::string& id)
    {
        // TODO: Implement note removal logic
    }

    void NoteManager::list_notes(std::string& parent_note_id) const
    {
        std::string title_like = std::string("");
        int page_size = 20;
        int page = 0;
        int note_number_as_child = 1;
        while (true)
        {
            auto list = db->note_repository->list(parent_note_id, page, page_size);
            if (list.empty())
            {
                if (page == 0)
                {
                    std::cout << "No notes found" << std::endl;
                }
                break;
            }
            int hash_length = std::to_string(note_number_as_child).size() + 2;
            int id_length = 0;
            {
                for (Entity::Note& e : list)
                {
                    int length = e.id.size();
                    if (length > id_length)
                    {
                        id_length = length;
                    }
                }
                id_length++;
            }
            std::cout
            << std::left
            << std::setw(hash_length + 1)
            << "#"
            << std::setw(id_length + 1)
            <<"ID"
            << "Title"
            << std::endl;
            for (Entity::Note& e : list)
            {
                std::cout
                << "#"
                << std::setw(hash_length)
                << note_number_as_child
                << std::setw(id_length)
                << e.id /*e.parent_note_id + ":" + e.get_last_id_segment()*/ << " " << e.title << std::endl;
                ++note_number_as_child;
            }
            ++page;
            if (list.size() != page_size || !Utils::ask_yes_no("Do you want to continue to the next page (y/n)? "))
            {
                break;
            }
        }
    }

    Entity::Content NoteManager::read_content(std::string& id)
    {
        return db->content_repository->read(id);

    }

    void NoteManager::update_content(Entity::Content& content, std::string& old_value)
    {
        db->content_repository->update(content);
        Entity::OldContent old_content;
        old_content.id = content.id;
        old_content.value = old_value;
        old_content.added_at = Utils::currentUnixTimestamp();
        db->old_content_repository->create(old_content);
    }

    void NoteManager::remove_content(const std::string& id)
    {
        // TODO: Implement content removal logic
    }
}
