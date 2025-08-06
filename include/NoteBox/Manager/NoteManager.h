//
// Created by robertvokac on 8/4/25.
//

#ifndef NOTEMANAGER_H
#define NOTEMANAGER_H
#include <memory>

#include "NoteBox/Entity/Note.h"

namespace NoteBox::Manager {
    class NoteManager {
    private:
        std::string currentPath; // e.g., "/1a/2b"

    public:
        NoteManager();

        const std::string &getCurrentPath() const;

        std::shared_ptr<Entity::Note> getCurrentNote();

        void cd(const std::string &path);

        void addNote(const std::string &title);

        void list(size_t page = 0, size_t pageSize = 20) const;

        void remove(const std::string &id);
    };
}


#endif //NOTEMANAGER_H
