//
// Created by robertvokac on 8/4/25.
//

#ifndef ICOMMAND_H
#define ICOMMAND_H
#include "ICommandHelper.h"
#include "MiniWiki/Manager/MiniWikiManager.h"

namespace MiniWiki::Command {
    class ICommand {
    public:

        virtual void execute(Manager::MiniWikiManager& mgr, const std::string& args) = 0;
        virtual void help() = 0;
        virtual void setCommandHelper(ICommandHelper* factory) {};
        virtual ICommandHelper* get_command_helper() {return nullptr;};
        virtual ~ICommand() = default;
    };

}

#endif //ICOMMAND_H
