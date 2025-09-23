#ifndef LOGGER_H
#define LOGGER_H

#include "mindnet/essential/ConsolePrinter.h"
#include <stdexcept>

#include "LogLevel.h"

namespace mindnet::essential
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
