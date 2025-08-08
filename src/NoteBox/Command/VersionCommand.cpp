//
// Created by robertvokac on 8/4/25.
//

#include "NoteBox/Command/VersionCommand.h"

#include <iostream>

namespace NoteBox::Command {
    void VersionCommand::execute(Manager::NoteManager &mgr, const std::string &args) {
        std::cout << "0.0.0-SNAPSHOT" << std::endl;
    }
    void VersionCommand::help() {
        std::cout << "help to be implemented" << std::endl;
    }
}
