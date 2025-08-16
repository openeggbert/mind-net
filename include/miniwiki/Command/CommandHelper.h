//
// Created by robertvokac on 8/4/25.
//
#ifndef COMMANDHELPER_H
#define COMMANDHELPER_H


#include "CommandFactory.h"
#include "ICommandHelper.h"

namespace MiniWiki::Command {
    class CommandHelper : public ICommandHelper {
    private:
        MiniWiki::Command::CommandFactory* factory = nullptr;
        std::shared_ptr<MiniWiki::Persistence::DB>* db = nullptr;

    public:
        CommandHelper(MiniWiki::Command::CommandFactory* factory, std::shared_ptr<MiniWiki::Persistence::DB>* db);
        ~CommandHelper() override;  // ✅ Declare the destructor
        void print_help(const std::string& command) override;
        std::vector<std::string> list_commands() override;
        void execute(Manager::MiniWikiManager &mgr, const std::string& cmd, const std::string& args) override;
        std::shared_ptr<MiniWiki::Persistence::DB>* getDB() override;


    };
}


#endif // COMMANDHELPER_H
