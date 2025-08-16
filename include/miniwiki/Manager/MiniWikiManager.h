//
// Created by robertvokac on 8/6/25.
//

#ifndef NOTEBOX_H
#define NOTEBOX_H
#include "IdeaManager.h"
#include "LiteratureSourceManager.h"
#include "NoteManager.h"
#include "SessionManager.h"
#include "miniwiki/models/OldEntity.h"

namespace miniwiki::Persistence
{
    class DB;
}

namespace miniwiki::Manager {
    class MiniWikiManager {
    private:
        std::shared_ptr<Persistence::DB> db;

    public:
        LiteratureSourceManager literature_source_manager;
        NoteManager note_manager;
        SessionManager session_manager;
        IdeaManager idea_manager;

        MiniWikiManager(const std::shared_ptr<Persistence::DB>& db);
        void create_old_entity(const Entity::OldEntity& old_entity);

    };

}


#endif //NOTEBOX_H
