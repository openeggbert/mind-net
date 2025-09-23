//
// Created by robertvokac on 9/8/25.
//

#include "mindnet/essential/LogLevel.h"

#include "mindnet/essential/Environment.h"

#include <stdexcept>
#include <string>

#include "mindnet/essential/ConsoleColor.h"

namespace mindnet::essential
{
    std::string log_level_to_string(const LogLevel& log_level)
    {
        switch (log_level)
        {
        case FATAL: return "FATAL";
        case ERROR: return "ERROR";
        case WARN: return "WARN";
        case INFO: return "INFO";
        case DEBUG: return "DEBUG";
        case TRACE: return "TRACE";
        case EXPERIMENT: return "EXPERIMENT";
        default: throw std::runtime_error("Unknown log level: " + std::to_string(log_level));
        }
    }

    ConsoleColor log_level_to_console_color(const LogLevel& log_level)
    {
        switch (log_level)
        {
        case FATAL:
        case ERROR: return ConsoleColor::RED;
        case WARN: return ConsoleColor::YELLOW;
        case INFO: return ConsoleColor::GREEN;
        case DEBUG: return ConsoleColor::BLUE;
        case TRACE: return ConsoleColor::PURPLE;
        case EXPERIMENT: return ConsoleColor::CYAN;
        default: throw std::runtime_error("Unknown log level: " + std::to_string(log_level));
        }
    }

    LogLevel string_to_log_level(const std::string& log_level)
    {
        if (log_level == "FATAL") return FATAL;
        if (log_level == "ERROR") return ERROR;
        if (log_level == "WARN") return WARN;
        if (log_level == "INFO") return INFO;
        if (log_level == "DEBUG") return DEBUG;
        if (log_level == "TRACE") return TRACE;
        if (log_level == "EXPERIMENT") return EXPERIMENT;
        throw std::runtime_error("Unknown log level: " + log_level);
    }
}
