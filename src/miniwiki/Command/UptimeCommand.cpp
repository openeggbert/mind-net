//
// Created by robertvokac on 8/4/25.
//

#include "miniwiki/Command/UptimeCommand.h"

#include <iostream>

#include "miniwiki/Global.h"

namespace MiniWiki::Command
{
    namespace Labels
    {
        static const std::string DAY = " day ";
        static const std::string DAYS = " days ";
        static const std::string HOUR = " hour ";
        static const std::string HOURS = " hours ";
        static const std::string MINUTE = " minute ";
        static const std::string MINUTES = " minutes ";
        static const std::string SECOND = " second";
        static const std::string SECONDS = " seconds";
    }

    void UptimeCommand::execute(Manager::MiniWikiManager& mgr, const std::string& args)
    {
        static const int SECONDS_PER_DAY = 24 * 60 * 60;
        static const int SECONDS_PER_HOUR = 60 * 60;
        static const int SECONDS_PER_MINUTE = 60;

        ll current_time = Utils::currentUnixTimestamp();
        ll elapsed_seconds = current_time - MiniWiki::start_time;
        short days = 0;
        short hours = 0;
        short minutes = 0;
        short seconds = 0;
        days = elapsed_seconds / SECONDS_PER_DAY;
        elapsed_seconds -= days * SECONDS_PER_DAY;
        hours = elapsed_seconds / SECONDS_PER_HOUR;
        elapsed_seconds -= hours * SECONDS_PER_HOUR;
        minutes = elapsed_seconds / SECONDS_PER_MINUTE;
        elapsed_seconds -= minutes * SECONDS_PER_MINUTE;
        seconds = elapsed_seconds;
        std::cout << "Uptime: ";
        if (days > 0)
        {
            std::cout << days << (days == 1 ? Labels::DAY : Labels::DAYS);
        }
        if (hours > 0)
        {
            std::cout << hours << (hours == 1 ? Labels::HOUR : Labels::HOURS);
        }
        if (minutes > 0)
        {
            std::cout << minutes << (minutes == 1 ? Labels::MINUTE : Labels::MINUTES);
        }
        std::cout << seconds << (seconds == 1 ? Labels::SECOND : Labels::SECONDS);
        std::cout << std::endl;
    }

    void UptimeCommand::help()     {
        std::cout << R"(
NAME
       uptime

SYNOPSIS
       uptime

DESCRIPTION
       uptime prints the elapsed time since the start of the miniwiki application.

EXAMPLES
            uptime... it has no argument

)" << std::endl;
    }
}
