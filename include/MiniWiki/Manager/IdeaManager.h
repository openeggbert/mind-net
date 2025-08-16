//
// Created by robertvokac on 8/4/25.
//
#ifndef IDEAMANAGER_H
#define IDEAMANAGER_H


#include <memory>

#include "MiniWiki/Entity/Idea.h"

namespace MiniWiki::Persistence
{
    class DB;
}

namespace MiniWiki::Manager {
    class IdeaManager {
    private:
        std::shared_ptr<Persistence::DB> db;

    public:
        IdeaManager(const std::shared_ptr<Persistence::DB>& db);

        void create_idea(const Entity::Idea& idea);

        // const std::string &pwd() const;
        //
        // void cd(const std::string &path);
        //
        // void create_note(const std::string &title);
        //
        // Entity::Note read_note(std::string& id);
        //
        // void update_note(Entity::Note& note);
        //
        // void remove_note(const std::string &id);
        //
        // void list_notes(std::string& parent_note_id) const;
        //
        // Entity::Content read_content(std::string& id);
        //
        // void update_content(Entity::Content& note, std::string& old_value);
        //
        // void remove_content(const std::string &id);

    };
}


#endif // IDEAMANAGER_H
