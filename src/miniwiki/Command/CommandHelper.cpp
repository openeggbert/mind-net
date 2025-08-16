//
// Created by robertvokac on 8/4/25.
//

#include "miniwiki/Command/CommandHelper.h"

#include <iostream>

#include "miniwiki/Global.h"

namespace miniwiki::Command
{
    CommandHelper::CommandHelper(miniwiki::Command::CommandFactory* factory_, std::shared_ptr<miniwiki::Persistence::DB>* db_)
    {
        this->factory = factory_;
        this->db = db_;
    }

    CommandHelper::~CommandHelper() = default;

    void CommandHelper::print_help(const std::string& cmd)
    {
        if (this->factory == nullptr)
        {
            throw std::runtime_error("factory is not set");
        }
        auto command = factory->getCommand(cmd);
        if (command)
        {
            command->help();
        }
        else
        {
            err << "Unknown command: " << cmd << "\n";
        }
    }

    std::vector<std::string> CommandHelper::list_commands()
    {
        return this->factory->list_commands();
    }

    void CommandHelper::execute(Manager::MiniWikiManager &mgr, const std::string& cmd, const std::string& args)
    {
        if (this->factory == nullptr)
        {
            throw std::runtime_error("factory is not set");
        }
        auto command = factory->getCommand(cmd);
        if (command)
        {
            command->execute(mgr, args);
        }
        else
        {
            err << "Unknown command: " << cmd << "\n";
        }
    }

    std::shared_ptr<miniwiki::Persistence::DB>* CommandHelper::getDB()
    {
        return db;
    }
}
