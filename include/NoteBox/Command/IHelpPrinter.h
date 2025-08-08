//
// Created by robertvokac on 8/4/25.
//
#ifndef IHELPPRINTER_H
#define IHELPPRINTER_H
#include <string>
#include <vector>

namespace NoteBox::Command
{
    class IHelpPrinter
    {
    public:
        virtual void print(const std::string& command) = 0;
        virtual std::vector<std::string> list_commands() = 0;
        virtual ~IHelpPrinter() = default;
    };
}

#endif // IHELPPRINTER_H
