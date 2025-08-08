//
// Created by robertvokac on 8/4/25.
//

#include "NoteBox/Command/CommandFactory.h"

#include "NoteBox/Command/AddCommand.h"
#include "NoteBox/Command/CdCommand.h"
#include "NoteBox/Command/HelpCommand.h"
#include "NoteBox/Command/LsCommand.h"
#include "NoteBox/Command/RmCommand.h"
#include "NoteBox/Command/VersionCommand.h"
#define add_command(key, clazz)         commands[#key] = std::make_shared<clazz##Command>();


namespace NoteBox::Command
{
    CommandFactory::CommandFactory()
    {

        add_command(version, Version)
        add_command(add, Add)
        add_command(cd, Cd)
        add_command(ls, Ls)
        add_command(rm, Rm)
        add_command(help, Help)
    }

    std::shared_ptr<ICommand> CommandFactory::getCommand(const std::string& name)
    {
        return commands.count(name) ? commands[name] : nullptr;
    }
}
