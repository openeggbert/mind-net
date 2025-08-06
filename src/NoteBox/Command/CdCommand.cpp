//
// Created by robertvokac on 8/4/25.
//

#include "NoteBox/Command/CdCommand.h"

#include <iostream>

namespace NoteBox::Command {
    void CdCommand::execute(Manager::NoteManager &mgr, const std::string &args) {
        mgr.cd(args);
        std::cout << ":" << mgr.getCurrentPath() << "\n";
    }
}
