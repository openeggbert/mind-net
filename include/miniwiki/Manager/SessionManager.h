//
// Created by robertvokac on 8/4/25.
//
#ifndef SESSIONMANAGER_H
#define SESSIONMANAGER_H

#include <memory>
#include "miniwiki/models/Session.h"
#include "miniwiki/persistence/DB.h"
#include "miniwiki/persistence/Impl/Sqlite/Repositories/SessionRepositoryImplSqlite.h"

namespace MiniWiki::Manager
{
    class SessionManager
    {
    private:
        std::shared_ptr<Persistence::DB> db;

    public:
        SessionManager(const std::shared_ptr<Persistence::DB>& db);

        void create(const Entity::Session& session);

        Entity::Session get();

        void update(const Entity::Session& session);
    };
}

#endif // SESSIONMANAGER_H
