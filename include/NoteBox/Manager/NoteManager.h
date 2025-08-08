//
// Created by robertvokac on 8/4/25.
//

#ifndef NOTEMANAGER_H
#define NOTEMANAGER_H
#include <memory>

#include "NoteBox/Entity/Content.h"
#include "NoteBox/Entity/Note.h"

namespace NoteBox::Persistence
{
    class DB;
}

namespace NoteBox::Manager {
    class NoteManager {
    private:
        std::string currentPath; // e.g., "/1a/2b"
        std::shared_ptr<Persistence::DB> db;

    public:
        NoteManager(const std::shared_ptr<Persistence::DB>& db);

        const std::string &pwd() const;

        void cd(const std::string &path);

        void createNote(const std::string &title);

        Entity::Note readNote(std::string& id);

        void updateNote(Entity::Note& note);

        void removeNote(const std::string &id);

        void listNotes(size_t page = 0, size_t pageSize = 20) const;

        void createContent(const std::string& Content);

        Entity::Note readContent(std::string& id);

        void updateContent(Entity::Content& note);

        void removeContent(const std::string &id);

    };
}


#endif //NOTEMANAGER_H
