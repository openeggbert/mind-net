//
// Created by robertvokac on 8/4/25.
//

#include "NoteBox/Manager/SessionManager.h"

#include <iostream>

namespace NoteBox::Manager
{
    SessionManager::SessionManager(const std::shared_ptr<Persistence::DB>& db_) : db(db_)
    {
    }

    void SessionManager::create(const Entity::Session& session)
    {
        db->session_repository->create(session);
    }

    Entity::Session SessionManager::get()
    {
        return db->session_repository->get();
    }

    void SessionManager::update(const Entity::Session& session)
    {
        db->session_repository->update(session);
    }
}
