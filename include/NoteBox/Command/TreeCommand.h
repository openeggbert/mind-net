//
// Created by robertvokac on 8/4/25.
//

#include "ICommand.h"

namespace NoteBox::Command {
    class TreeCommand : public ICommand {

        ICommandHelper* helper = nullptr;

    public:
        void execute(Manager::NoteBoxManager& mgr, const std::string& args) override;
        void help() override;
        void setCommandHelper(ICommandHelper* factory) override;
        ICommandHelper* get_command_helper() override;
    };
}
