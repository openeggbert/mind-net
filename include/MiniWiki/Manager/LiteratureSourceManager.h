//
// Created by robertvokac on 8/4/25.
//
#ifndef LITERATURESOURCEMANAGER_H
#define LITERATURESOURCEMANAGER_H


#include <memory>
#include <vector>

#include "MiniWiki/Entity/LiteratureSource.h"
#include "MiniWiki/Persistence/DB.h"
#include "MiniWiki/Persistence/Impl/Sqlite/Repositories/LiteratureSourceRepositoryImplSqlite.h"

namespace MiniWiki::Manager
{
    class LiteratureSourceManager
    {
    private:
        std::shared_ptr<Persistence::DB> db;
    public:
        LiteratureSourceManager(const std::shared_ptr<Persistence::DB>& db);

        void create();
        Entity::LiteratureSource read(ushort id);
        std::vector<Entity::LiteratureSource> list();

        void remove(const Entity::LiteratureSource& literature_source);

        void update(Entity::LiteratureSource& literature_source);
    };
}

#endif // LITERATURESOURCEMANAGER_H
