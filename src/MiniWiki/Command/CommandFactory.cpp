//
// Created by robertvokac on 8/4/25.
//

#include "MiniWiki/Command/CommandFactory.h"

#include "MiniWiki/Command/AddCommand.h"
#include "MiniWiki/Command/CdCommand.h"
#include "MiniWiki/Command/HelpCommand.h"
#include "MiniWiki/Command/LsCommand.h"
#include "MiniWiki/Command/RmCommand.h"
#include "MiniWiki/Command/VersionCommand.h"
#include "MiniWiki/Command/LitCommand.h"
#include "MiniWiki/Command/PwdCommand.h"
#include "MiniWiki/Command/UptimeCommand.h"
#include "MiniWiki/Command/EditorCommand.h"
#include "MiniWiki/Command/NameCommand.h"
#include "MiniWiki/Command/InfoCommand.h"
#include "MiniWiki/Command/ShowCommand.h"
#include "MiniWiki/Command/WalkCommand.h"
#include "MiniWiki/Command/EditCommand.h"
#include "MiniWiki/Command/UpdateCommand.h"
#include "MiniWiki/Command/TreeCommand.h"
#include "MiniWiki/Command/TermCommand.h"
#include "MiniWiki/Command/IdeaCommand.h"
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
