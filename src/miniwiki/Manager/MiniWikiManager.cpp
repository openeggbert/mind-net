//
// Created by robertvokac on 8/6/25.
//

#include "miniwiki/Manager/MiniWikiManager.h"

namespace MiniWiki::Manager
{
    MiniWikiManager::MiniWikiManager(const std::shared_ptr<Persistence::DB>& db)
        :
    literature_source_manager(db),
    note_manager(db),
    session_manager(db),
    idea_manager(db),
    db(db)
    //, tags(db), collections(db), references(db)
    {
    }

    void MiniWikiManager::create_old_entity(const Entity::OldEntity& old_entity)
    {
        db->old_entity_repository->create(old_entity);
    }
    ;
}
