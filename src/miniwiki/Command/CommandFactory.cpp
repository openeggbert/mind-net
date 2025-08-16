//
// Created by robertvokac on 8/4/25.
//

#include "miniwiki/Command/CommandFactory.h"

#include "miniwiki/Command/AddCommand.h"
#include "miniwiki/Command/CdCommand.h"
#include "miniwiki/Command/HelpCommand.h"
#include "miniwiki/Command/LsCommand.h"
#include "miniwiki/Command/RmCommand.h"
#include "miniwiki/Command/VersionCommand.h"
#include "miniwiki/Command/LitCommand.h"
#include "miniwiki/Command/PwdCommand.h"
#include "miniwiki/Command/UptimeCommand.h"
#include "miniwiki/Command/EditorCommand.h"
#include "miniwiki/Command/NameCommand.h"
#include "miniwiki/Command/InfoCommand.h"
#include "miniwiki/Command/ShowCommand.h"
#include "miniwiki/Command/WalkCommand.h"
#include "miniwiki/Command/EditCommand.h"
#include "miniwiki/Command/UpdateCommand.h"
#include "miniwiki/Command/TreeCommand.h"
#include "miniwiki/Command/TermCommand.h"
#include "miniwiki/Command/IdeaCommand.h"
#define add(key, clazz) commands[#key] = std::make_shared<clazz##Command>();commandNames.push_back(#key);


namespace MiniWiki::Command
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
