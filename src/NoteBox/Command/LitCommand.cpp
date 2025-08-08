//
// Created by robertvokac on 8/4/25.
//

#include "NoteBox/Command/LitCommand.h"

#include <iostream>

//#define genif(cmd, method) if (args == #cmd){mgr.literature_source_manager.##method();return;}
namespace NoteBox::Command
{
    void LitCommand::execute(Manager::NoteBoxManager& mgr, const std::string& args)
    {
        std::cout << "hello lit command" << std::endl;
        if (args.empty())
        {
            std::cerr << "lit command requires an argument" << std::endl;
            return;
        }
        if (args == "ls")
        {
            mgr.literature_source_manager.list();
            return;
        }
        if (args == "add")
        {
            mgr.literature_source_manager.create();
            return;
        }
        if (args == "edit")
        {
            mgr.literature_source_manager.list();
            return;
        }
        if (args == "show")
        {
            mgr.literature_source_manager.list();
            return;
        }

        std::cerr << "lit command " << args << " not implemented" << std::endl;
    }

    void LitCommand::help()
    {
        std::cout << "help to be implemented" << std::endl;
    }
}
