//
// Created by robertvokac on 8/4/25.
//

#include "MiniWiki/Command/CommandHelper.h"

#include <iostream>

#include "MiniWiki/Global.h"

namespace MiniWiki::Command
{
    CommandHelper::CommandHelper(MiniWiki::Command::CommandFactory* factory_, std::shared_ptr<MiniWiki::Persistence::DB>* db_)
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

    std::shared_ptr<MiniWiki::Persistence::DB>* CommandHelper::getDB()
    {
        return db;
    }
}
