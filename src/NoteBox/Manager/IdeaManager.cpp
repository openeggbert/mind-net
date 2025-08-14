// Created by robertvokac on 8/4/25.

#include "NoteBox/Manager/IdeaManager.h"

#include <iomanip>
#include <iostream>

#include "NoteBox/Global.h"
#include "NoteBox/RedCout.h"
#include "NoteBox/Utils.h"
#include "NoteBox/Persistence/DB.h"

namespace NoteBox::Manager
{
    IdeaManager::IdeaManager(const std::shared_ptr<NoteBox::Persistence::DB>& db_)
        : db(db_)
    {
    }

    void IdeaManager::create_idea(const Entity::Idea& idea)
    {
        db->idea_repository->create(idea);
    }
}
