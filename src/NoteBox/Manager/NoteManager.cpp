//
// Created by robertvokac on 8/4/25.
//

#include "NoteBox/Manager/NoteManager.h"

namespace NoteBox::Manager {
    NoteManager::NoteManager(const std::shared_ptr<Persistence::DB>& db) : currentPath("/") {
    }

    const std::string &NoteManager::getCurrentPath() const { return currentPath; }

    std::shared_ptr<Entity::Note> NoteManager::getCurrentNote() {
        return nullptr;//notes.at(currentPath);
    }

    void NoteManager::cd(const std::string &path) {
        if (path == "..") {
            if (currentPath != "/") {
                auto pos = currentPath.find_last_of("/");
                currentPath = currentPath.substr(0, pos == 0 ? 1 : pos);
            }
        } else {
            std::string next = currentPath == "/" ? "/" + path : currentPath + path;
            // if (notes.count(next)) {
            //     currentPath = next;
            // } else {
            //     std::cout << "No such note: " << next << "\n";
            // }
        }
    }

    void NoteManager::addNote(const std::string &title) {
        // auto parent = getCurrentNote();
        //
        // // Generate next sub-ID: e.g. /1a, /1b, /1c, skipping deleted
        // char suffix = 'a';
        // std::string newId;
        // while (true) {
        //     newId = (currentPath == "/" ? "/" : currentPath) + (currentPath == "/" ? std::to_string(notes.size()) : "")
        //             + suffix;
        //     // if (!notes.count(newId)) break;
        //     ++suffix;
        // }
        //
        // auto newNote = std::make_shared<Note>(newId, title);
        // parent->children[newId] = newNote;
        // notes[newId] = newNote;
        // currentPath = newId;
        // std::cout << ":" << currentPath << "\n";
    }

    void NoteManager::list(size_t page, size_t pageSize) const {
        // auto note = getCurrentNote();
        // auto it = note->children.begin();
        // std::advance(it, page * pageSize);
        //
        // size_t count = 0;
        // for (; it != note->children.end() && count < pageSize; ++it, ++count) {
        //     std::cout << it->first << " - " << it->second->title << '\n';
        // }
        //
        // size_t total = note->children.size();
        // size_t shown = std::min((page + 1) * pageSize, total);
        //
        // if (shown < total) {
        //     std::cout << "[n]ext page / [q]uit: ";
        //     std::string choice;
        //     std::getline(std::cin, choice);
        //     if (choice == "n") {
        //         list(page + 1, pageSize);
        //     }
        }

    void NoteManager::remove(const std::string &id) {
        std::string fullId = currentPath == "/" ? "/" + id : currentPath + id;
        // if (notes.count(fullId)) {
        //     getCurrentNote()->children.erase(fullId);
        //     notes.erase(fullId);
        //     std::cout << "Deleted: " << fullId << "\n";
        // } else {
        //     std::cout << "Note not found: " << fullId << "\n";
        // }
    };
}
