//
// Created by robertvokac on 8/4/25.
//
#ifndef IHELPPRINTER_H
#define IHELPPRINTER_H
#include <string>
#include <vector>

#include "miniwiki/Manager/MiniWikiManager.h"

namespace miniwiki::Command
{
    class ICommandHelper
    {
    public:
        virtual void print_help(const std::string& command) = 0;
        virtual std::vector<std::string> list_commands() = 0;
        virtual void execute(Manager::MiniWikiManager &mgr, const std::string& cmd, const std::string& args) = 0;
        virtual std::shared_ptr<Persistence::DB>* getDB() = 0;

        virtual ~ICommandHelper() = default;
    };
}

#endif // IHELPPRINTER_H
