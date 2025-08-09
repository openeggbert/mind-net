//
// Created by robertvokac on 8/4/25.
//

#include "NoteBox/Command/CommandHelper.h"

#include <iostream>

#include "NoteBox/Global.h"

namespace NoteBox::Command
{
    CommandHelper::CommandHelper(NoteBox::Command::CommandFactory* factory_, std::shared_ptr<NoteBox::Persistence::DB>* db_)
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

    void CommandHelper::execute(Manager::NoteBoxManager &mgr, const std::string& cmd, const std::string& args)
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

    std::shared_ptr<NoteBox::Persistence::DB>* CommandHelper::getDB()
    {
        return db;
    }
}
