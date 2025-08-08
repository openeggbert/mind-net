//
// Created by robertvokac on 8/4/25.
//

#include "NoteBox/Command/LsCommand.h"

#include <iostream>

namespace NoteBox::Command {
    void LsCommand::execute(Manager::NoteManager &mgr, const std::string &args) {
        mgr.list();
    }
    void LsCommand::help() {
        std::cout << "help to be implemented" << std::endl;
    }
}
