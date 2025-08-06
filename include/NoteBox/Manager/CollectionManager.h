//
// Created by robertvokac on 8/4/25.
//
#ifndef COLLECTIONMANAGER_H
#define COLLECTIONMANAGER_H

#include <memory>

#include "NoteBox/Entity/Note.h"

namespace NoteBox::Manager {
    class CollectionManager {
    private:
        std::string currentPath; // e.g., "/1a/2b"

    public:
        CollectionManager();

        const std::string &getCurrentPath() const;

        std::shared_ptr<Enums::Note> getCurrentNote();

        void cd(const std::string &path);

        void addNote(const std::string &title);

        void list(size_t page = 0, size_t pageSize = 20) const;

        void remove(const std::string &id);
    };
}

#endif // COLLECTIONMANAGER_H
