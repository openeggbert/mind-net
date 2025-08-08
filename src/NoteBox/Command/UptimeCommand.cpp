//
// Created by robertvokac on 8/4/25.
//

#include "NoteBox/Command/UptimeCommand.h"

#include <iostream>

#include "NoteBox/Global.h"

namespace NoteBox::Command {
    void UptimeCommand::execute(Manager::NoteBoxManager &mgr, const std::string &args) {
        ll current_time = Utils::currentUnixTimestamp();
        ll elapsed_seconds = current_time - NoteBox::start_time;
        short days = 0;
        short hours = 0;
        short minutes = 0;
        short seconds = 0;
        days = elapsed_seconds / (24 * 60 * 60);
        elapsed_seconds -= days * (24 * 60 * 60);
        hours = elapsed_seconds / (60 * 60);
        elapsed_seconds -= hours * (60 * 60);
        minutes = elapsed_seconds / (60);
        elapsed_seconds -= minutes * (60);
        seconds = elapsed_seconds;
        std::cout << "Uptime: ";
        if (days > 0)
        {
            std::cout << days<< " days ";
        }
        if (hours > 0)
        {
            std::cout << hours<< " hours ";
        }
        if (minutes > 0)
        {
            std::cout << minutes<< " minutes ";
        }
        std::cout << seconds<< " seconds";
        std::cout << std::endl;

    }
    void UptimeCommand::help()     {
        std::cout << R"(
NAME
       uptime

SYNOPSIS
       uptime

DESCRIPTION
       uptime prints the elapsed time since the start of the notebox application.

EXAMPLES
       uptime ... it has no argument

)" << std::endl;
    }
}
