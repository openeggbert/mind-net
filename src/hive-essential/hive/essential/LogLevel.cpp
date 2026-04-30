/*
 * MIT License
 * Copyright (c) 2025 Robert Vokac
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include "hive/essential/LogLevel.hpp"

#include "hive/essential/Environment.hpp"

#include <stdexcept>
#include <string>

#include "hive/essential/ConsoleColor.hpp"

namespace hive::essential
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