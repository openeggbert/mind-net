//
// Created by robertvokac on 8/4/25.
//

#ifndef NOTEMANAGER_H
#define NOTEMANAGER_H
#include <memory>

#include "miniwiki/models/Content.h"
#include "miniwiki/models/Note.h"

namespace MiniWiki::Persistence
{
    class DB;
}

namespace MiniWiki::Manager {
    class NoteManager {
    private:
        std::string currentPath; // e.g., "/1a/2b"
        std::shared_ptr<Persistence::DB> db;

    public:
        NoteManager(const std::shared_ptr<Persistence::DB>& db);

        const std::string &pwd() const;

        void cd(const std::string &path);

        void create_note(const std::string &title);

        Entity::Note read_note(std::string& id);

        void update_note(Entity::Note& note);

        void remove_note(const std::string &id);

        void list_notes(std::string& parent_note_id) const;

        Entity::Content read_content(std::string& id);

        void update_content(Entity::Content& note, std::string& old_value);

        void remove_content(const std::string &id);

    };
}


#endif //NOTEMANAGER_H
