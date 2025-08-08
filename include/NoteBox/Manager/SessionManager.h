//
// Created by robertvokac on 8/4/25.
//
#ifndef SESSIONMANAGER_H
#define SESSIONMANAGER_H

#include <memory>
#include "NoteBox/Entity/Session.h"
#include "NoteBox/Persistence/DB.h"
#include "NoteBox/Persistence/Impl/Sqlite/Repositories/SessionRepositoryImplSqlite.h"

namespace NoteBox::Manager
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
