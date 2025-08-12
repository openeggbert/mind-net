//
// Created by robertvokac on 8/4/25.
//
#ifndef UPDATECOMMAND_H
#define UPDATECOMMAND_H


#include "ICommand.h"

namespace NoteBox::Command {
    class UpdateCommand : public ICommand {
    public:
        bool get_note_id_from_args(Manager::NoteBoxManager& mgr, const std::string& args, std::string& path);
        void execute(Manager::NoteBoxManager& mgr, const std::string& args) override;
        void help() override;
    };
}

#endif // UPDATECOMMAND_H
