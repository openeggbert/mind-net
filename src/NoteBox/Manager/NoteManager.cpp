// Created by robertvokac on 8/4/25.

#include "NoteBox/Manager/NoteManager.h"

#include <iostream>

#include "NoteBox/Global.h"
#include "NoteBox/RedCout.h"
#include "NoteBox/Utils.h"
#include "NoteBox/Enum/Importance.h"
#include "NoteBox/Enum/Difficulty.h"
#include "NoteBox/Persistence/DB.h"

namespace NoteBox::Manager
{
    NoteManager::NoteManager(const std::shared_ptr<Persistence::DB>& db)
        : currentPath(""), db(db)
    {
    }

    const std::string& NoteManager::pwd() const
    {
        return currentPath;
    }

    void NoteManager::cd(const std::string& path)
    {
        if (path.empty())
        {
            currentPath = "";
            return;
        }
        if (path == "..")
        {
            auto parts = Utils::note_id_to_vector(currentPath);
            parts.pop_back();
            currentPath = Utils::vector_to_note_id(parts);
            return;
        }
        if (!db->note_repository->does_id_exist(path))
        {
            err << "Note with id " << path << " does not exist" << std::endl;
            return;
        }
        currentPath = path;
    }

    void NoteManager::createNote(const std::string& title)
    {
        Entity::Note note;
        using std::cout;
        using std::cin;
        using std::endl;
        typedef std::string s;


        s current_path = pwd();
        auto vector_path = current_path.empty() ? std::vector<std::string>{} : Utils::note_id_to_vector(current_path);
        if (!vector_path.empty()) { vector_path.pop_back(); }
        auto parent_note_id = Utils::vector_to_note_id(vector_path);
        //
        Entity::Content content;
        s question;
        cout << "Question: ";
        getline(cin,question); cin.clear();

        //

        note.id = "";//todo
        note.parent_note_id = parent_note_id;
        note.title = title;
        //
        content.id = note.id;
        content.value = Utils::editTextInEditor(content.value, db->session_repository->get().editor_path);
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
        note.importance = static_cast<int>(NoteBox::Enum::Importance::MEDIUM);
        note.difficulty = static_cast<int>(NoteBox::Enum::Difficulty::MEDIUM);
        if (!NoteBox::Utils::ask_yes_no("Do you really want to create this new Note?")) {return;}
        db->note_repository->create(note);
    }

    Entity::Note NoteManager::readNote(std::string& id)
    {
        // TODO: Implement note reading logic
        return Entity::Note{};
    }

    void NoteManager::updateNote(Entity::Note& note)
    {
        // TODO: Implement note update logic
    }

    void NoteManager::removeNote(const std::string& id)
    {
        // TODO: Implement note removal logic
    }

    void NoteManager::listNotes(std::string& parent_note_id) const
    {
        std::string title_like = std::string("");
        int page_size = 20;
        int page = 0;
        int note_number_as_child = 1;
        while (true)
        {
            auto list = db->note_repository->list(parent_note_id, page, page_size);
            if (list.empty()) { break; }
            for (Entity::Note& e : list)
            {
                std::cout << note_number_as_child << " | " << e.id << " | " << e.title << std::endl;
                ++note_number_as_child;
            }
            ++page;
            if (list.size() != page_size || !Utils::ask_yes_no("Do you want to continue to the next page (y/n)? "))
            {
                break;
            }
        }
    }

    void NoteManager::createContent(const std::string& content)
    {
        // TODO: Implement content creation logic
    }

    Entity::Note NoteManager::readContent(std::string& id)
    {
        // TODO: Implement content reading logic
        return {};
    }

    void NoteManager::updateContent(Entity::Content& note)
    {
        // TODO: Implement content update logic
    }

    void NoteManager::removeContent(const std::string& id)
    {
        // TODO: Implement content removal logic
    }
}
