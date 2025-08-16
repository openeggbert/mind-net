// Created by robertvokac on 8/4/25.

#include "miniwiki/Manager/IdeaManager.h"

#include <iomanip>
#include <iostream>

#include "miniwiki/Global.h"
#include "miniwiki/RedCout.h"
#include "miniwiki/Utils.h"
#include "miniwiki/persistence/DB.h"

namespace miniwiki::Manager
{
    IdeaManager::IdeaManager(const std::shared_ptr<miniwiki::Persistence::DB>& db_)
        : db(db_)
    {
    }

    void IdeaManager::create_idea(const Entity::Idea& idea)
    {
        db->idea_repository->create(idea);
    }
}
