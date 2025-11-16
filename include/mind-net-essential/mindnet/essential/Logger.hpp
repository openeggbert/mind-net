#pragma once


#include "mindnet/essential/ConsolePrinter.hpp"
#include <stdexcept>

#include "LogLevel.hpp"

namespace mindnet::essential
{
    typedef ConsolePrinter logger;

    class Logger : public ConsolePrinter
    {
        LogLevel level;

    public:
        Logger(LogLevel log_level);
        bool is_enabled();
    };
}

