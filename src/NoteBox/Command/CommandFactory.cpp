//
// Created by robertvokac on 8/4/25.
//

#include "NoteBox/Command/CommandFactory.h"

#include "NoteBox/Command/VersionCommand.h"

namespace NoteBox::Command {
    CommandFactory::CommandFactory() {
        commands["version"] = std::make_shared<VersionCommand>();
        // commands["new"] = std::make_shared<NewCommand>();
        // commands["cd"] = std::make_shared<CdCommand>();
        // commands["ls"] = std::make_shared<LsCommand>();
        // commands["rm"] = std::make_shared<RmCommand>();
    }

    std::shared_ptr<ICommand> CommandFactory::getCommand(const std::string &name) {
        return commands.count(name) ? commands[name] : nullptr;
    }
}
