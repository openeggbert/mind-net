// Created by robertvokac on 8/4/25.

#include "MiniWiki/Manager/IdeaManager.h"

#include <iomanip>
#include <iostream>

#include "MiniWiki/Global.h"
#include "MiniWiki/RedCout.h"
#include "MiniWiki/Utils.h"
#include "MiniWiki/Persistence/DB.h"

namespace MiniWiki::Manager
{
    IdeaManager::IdeaManager(const std::shared_ptr<MiniWiki::Persistence::DB>& db_)
        : db(db_)
    {
    }

    void IdeaManager::create_idea(const Entity::Idea& idea)
    {
        db->idea_repository->create(idea);
    }
}
