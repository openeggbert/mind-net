//
// Created by robertvokac on 8/4/25.
//

#ifndef COMMANDFACTORY_H
#define COMMANDFACTORY_H
#include <map>
#include <memory>
#include <string>

#include "NoteBox/Command/ICommand.h"


namespace NoteBox::Command {
    class CommandFactory {
        std::map<std::string, std::shared_ptr<ICommand> > commands;

    public:
        CommandFactory();

        std::shared_ptr<ICommand> getCommand(const std::string &name);
    };
}


#endif //COMMANDFACTORY_H
