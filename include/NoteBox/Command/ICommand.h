//
// Created by robertvokac on 8/4/25.
//

#ifndef ICOMMAND_H
#define ICOMMAND_H
#include "ICommandHelper.h"
#include "NoteBox/Manager/NoteBoxManager.h"

namespace NoteBox::Command {
    class ICommand {
    public:

        virtual void execute(Manager::NoteBoxManager& mgr, const std::string& args) = 0;
        virtual void help() = 0;
        virtual void setCommandHelper(ICommandHelper* factory) {};
        virtual ICommandHelper* get_command_helper() {return nullptr;};
        virtual ~ICommand() = default;
    };

}

#endif //ICOMMAND_H
