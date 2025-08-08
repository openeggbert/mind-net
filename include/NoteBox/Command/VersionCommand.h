//
// Created by robertvokac on 8/4/25.
//

#ifndef VERSIONCOMMAND_H
#define VERSIONCOMMAND_H
#include "ICommand.h"

namespace NoteBox::Command {
    class VersionCommand : public ICommand {
    public:
        void execute(Manager::NoteManager& mgr, const std::string& args) override;
        void help() override;
    };
}

#endif //VERSIONCOMMAND_H
