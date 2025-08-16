//
// Created by robertvokac on 8/4/25.
//
#ifndef SESSIONMANAGER_H
#define SESSIONMANAGER_H

#include <memory>
#include "MiniWiki/Entity/Session.h"
#include "MiniWiki/Persistence/DB.h"
#include "MiniWiki/Persistence/Impl/Sqlite/Repositories/SessionRepositoryImplSqlite.h"

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
