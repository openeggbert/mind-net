#ifndef LOGGER_H
#define LOGGER_H

#include "../core/ConsolePrinter.h"
#include <stdexcept>

#include "LogLevel.h"
#include "../Utils.h"

namespace mindnet::core
{
    typedef ConsolePrinter logger;

    class Logger : public ConsolePrinter
    {
        LogLevel level;

    public:
        Logger(LogLevel log_level);
    };
}
#endif // LOGGER_H
