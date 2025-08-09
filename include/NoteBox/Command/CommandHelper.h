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
    public:
        CommandHelper(NoteBox::Command::CommandFactory* factory);
        ~CommandHelper() override;  // ✅ Declare the destructor
        void print_help(const std::string& command) override;
        std::vector<std::string> list_commands() override;
        void execute(Manager::NoteBoxManager &mgr, const std::string& cmd, const std::string& args) override;


    };
}


#endif // COMMANDHELPER_H
