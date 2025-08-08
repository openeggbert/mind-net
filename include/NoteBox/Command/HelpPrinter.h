//
// Created by robertvokac on 8/4/25.
//
#ifndef HELPPRINTER_H
#define HELPPRINTER_H


#include "CommandFactory.h"
#include "IHelpPrinter.h"

namespace NoteBox::Command {
    class HelpPrinter : public IHelpPrinter {
    private:
        NoteBox::Command::CommandFactory* factory = nullptr;
    public:
        HelpPrinter(NoteBox::Command::CommandFactory* factory);
        ~HelpPrinter() override;  // ✅ Declare the destructor
        void print(const std::string& command) override;
        std::vector<std::string> list_commands() override;

    };
}


#endif // HELPPRINTER_H
