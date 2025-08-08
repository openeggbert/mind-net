// Created by robertvokac on 8/4/25.

#include "NoteBox/Manager/NoteManager.h"

#include <iostream>

#include "NoteBox/Global.h"
#include "NoteBox/RedCout.h"
#include "NoteBox/Utils.h"
#include "NoteBox/Persistence/DB.h"

namespace NoteBox::Manager {

    NoteManager::NoteManager(const std::shared_ptr<Persistence::DB>& db)
        : currentPath("/"), db(db) {}

    const std::string& NoteManager::pwd() const {
        return currentPath;
    }

    void NoteManager::cd(const std::string& path) {
        if (path.empty())
        {
            currentPath = "";
            return;
        }
        if (path == "..") {
            auto parts = Utils::note_id_to_vector(path);
            parts.pop_back();
            currentPath = Utils::vector_to_note_id(parts);
            return;
        }
        if (!db->note_repository->does_id_exist(path))
        {
            err << "Note with id " << path << " does not exist" << std::endl;
            return;
        }
        currentPath =  path;
    }

    void NoteManager::createNote(const std::string& title) {
        // TODO: Implement note creation logic
    }

    Entity::Note NoteManager::readNote(std::string& id) {
        // TODO: Implement note reading logic
        return Entity::Note{};
    }

    void NoteManager::updateNote(Entity::Note& note) {
        // TODO: Implement note update logic
    }

    void NoteManager::removeNote(const std::string& id) {
        // TODO: Implement note removal logic
    }

    void NoteManager::listNotes(size_t page, size_t pageSize) const {
        // TODO: Implement note listing with pagination
    }

    void NoteManager::createContent(const std::string& content) {
        // TODO: Implement content creation logic
    }

    Entity::Note NoteManager::readContent(std::string& id) {
        // TODO: Implement content reading logic
        return {};
    }

    void NoteManager::updateContent(Entity::Content& note) {
        // TODO: Implement content update logic
    }

    void NoteManager::removeContent(const std::string& id) {
        // TODO: Implement content removal logic
    }

}
