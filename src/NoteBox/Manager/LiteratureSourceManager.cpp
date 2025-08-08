//
// Created by robertvokac on 8/4/25.
//

#include "NoteBox/Manager/LiteratureSourceManager.h"

namespace NoteBox::Manager
{
    LiteratureSourceManager::LiteratureSourceManager(const std::shared_ptr<Persistence::DB>& db_) : db(db_)
    {
    }

    void LiteratureSourceManager::create(const Entity::LiteratureSource& literature_source)
    {
        db->literature_source_repository->create(literature_source);
    }

    std::vector<Entity::LiteratureSource> LiteratureSourceManager::list()
    {
        return db->literature_source_repository->list();
    }

    void LiteratureSourceManager::remove(const Entity::LiteratureSource& literature_source)
    {
        db->literature_source_repository->remove(literature_source);
    }

    void LiteratureSourceManager::update(Entity::LiteratureSource& literature_source)
    {
        db->literature_source_repository->update(literature_source);
    }
}
