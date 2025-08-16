//
// Created by robertvokac on 8/4/25.
//
#ifndef COMMANDHELPER_H
#define COMMANDHELPER_H


#include "CommandFactory.h"
#include "ICommandHelper.h"

namespace miniwiki::Command {
    class CommandHelper : public ICommandHelper {
    private:
        miniwiki::Command::CommandFactory* factory = nullptr;
        std::shared_ptr<miniwiki::Persistence::DB>* db = nullptr;

    public:
        CommandHelper(miniwiki::Command::CommandFactory* factory, std::shared_ptr<miniwiki::Persistence::DB>* db);
        ~CommandHelper() override;  // ✅ Declare the destructor
        void print_help(const std::string& command) override;
        std::vector<std::string> list_commands() override;
        void execute(Manager::MiniWikiManager &mgr, const std::string& cmd, const std::string& args) override;
        std::shared_ptr<miniwiki::Persistence::DB>* getDB() override;


    };
}


#endif // COMMANDHELPER_H
