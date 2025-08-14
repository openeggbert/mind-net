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
#include "NoteBox/Command/LitCommand.h"
#include "NoteBox/Command/PwdCommand.h"
#include "NoteBox/Command/UptimeCommand.h"
#include "NoteBox/Command/EditorCommand.h"
#include "NoteBox/Command/NameCommand.h"
#include "NoteBox/Command/InfoCommand.h"
#include "NoteBox/Command/ShowCommand.h"
#include "NoteBox/Command/WalkCommand.h"
#include "NoteBox/Command/EditCommand.h"
#include "NoteBox/Command/UpdateCommand.h"
#include "NoteBox/Command/TreeCommand.h"
#include "NoteBox/Command/TermCommand.h"
#include "NoteBox/Command/IdeaCommand.h"
#define add(key, clazz) commands[#key] = std::make_shared<clazz##Command>();commandNames.push_back(#key);


namespace NoteBox::Command
{
    CommandFactory::CommandFactory()
    {

        add(help, Help)
        add(version, Version)
        add(walk, Walk)
        add(add, Add)
        add(name, Name)
        add(info, Info)
        add(show, Show)
        add(edit, Edit)
        add(lit, Lit)
        add(rm, Rm)
        add(cd, Cd)
        add(ls, Ls)
        add(pwd, Pwd)
        add(uptime, Uptime)
        add(editor, Editor)
        add(update, Update)
        add(tree, Tree)
        add(term, Term)
        add(idea, Idea)
    }

    std::shared_ptr<ICommand> CommandFactory::getCommand(const std::string& name)
    {
        return commands.count(name) ? commands[name] : nullptr;
    }

    std::vector<std::string>& CommandFactory::list_commands()
    {
        return commandNames;
    }
}
