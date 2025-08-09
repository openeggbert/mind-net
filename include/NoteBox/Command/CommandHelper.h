//
// Created by robertvokac on 8/4/25.
//
#ifndef COMMANDHELPER_H
#define COMMANDHELPER_H


#include "CommandFactory.h"
#include "ICommandHelper.h"

namespace NoteBox::Command {
    class CommandHelper : public ICommandHelper {
    private:
        NoteBox::Command::CommandFactory* factory = nullptr;
        std::shared_ptr<NoteBox::Persistence::DB>* db = nullptr;

    public:
        CommandHelper(NoteBox::Command::CommandFactory* factory, std::shared_ptr<NoteBox::Persistence::DB>* db);
        ~CommandHelper() override;  // ✅ Declare the destructor
        void print_help(const std::string& command) override;
        std::vector<std::string> list_commands() override;
        void execute(Manager::NoteBoxManager &mgr, const std::string& cmd, const std::string& args) override;
        std::shared_ptr<NoteBox::Persistence::DB>* getDB() override;


    };
}


#endif // COMMANDHELPER_H
