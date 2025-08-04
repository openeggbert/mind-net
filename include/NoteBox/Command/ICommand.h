//
// Created by robertvokac on 8/4/25.
//

#ifndef ICOMMAND_H
#define ICOMMAND_H
#include "NoteBox/Manager/NoteManager.h"

namespace NoteBox::Command {
    class ICommand {
    public:
        virtual void execute(Manager::NoteManager& mgr, const std::string& args) = 0;
        virtual ~ICommand() = default;
    };

}



#endif //ICOMMAND_H
