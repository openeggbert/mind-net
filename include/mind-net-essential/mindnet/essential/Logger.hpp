#ifndef LOGGER_H
#define LOGGER_H

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
#endif // LOGGER_H
