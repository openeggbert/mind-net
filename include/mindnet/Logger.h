#ifndef LOGGER_H
#define LOGGER_H

#include "ConsolePrinter.h"
#include <stdexcept>
#include "Utils.h"

namespace mindnet
{
    typedef ConsolePrinter logger;
    using std::endl;

    /**
     * @enum LogLevel
     * @brief Represents the severity of a log message.
     *
     * Making sense out of logs is not an easy task. Logs come from multiple sources,
     * with different structures and granularity. Using meaningful log levels helps
     * developers and operators understand the importance of messages quickly and reduce
     * alert fatigue.
     *
     * Log levels indicate how important a given log message is. They allow filtering
     * of critical information from purely informative events, helping to diagnose issues,
     * monitor application behavior, and set up alerts for crucial problems.
     *
     * The hierarchy of log levels in this enumeration is as follows:
     *
     * TRACE   - Very fine-grained information, mostly used for debugging deep internal
     *           application or library behavior. This level is highly verbose.
     *
     * DEBUG   - Less granular than TRACE but still useful for troubleshooting and
     *           verifying that the application runs correctly, especially in test
     *           environments.
     *
     * INFO    - Standard level indicating that an event happened, such as entering a
     *           certain state. Purely informative; normal operations do not depend
     *           on inspecting these messages.
     *
     * WARN    - Indicates unexpected situations that do not prevent the application
     *           from continuing to work, but may require attention. For example, a
     *           non-critical parsing error.
     *
     * ERROR   - Signals a problem that prevents one or more functionalities from
     *           working correctly. For example, a subsystem is unavailable but the
     *           application can still continue partially.
     *
     * FATAL   - Critical level indicating that a major business functionality is
     *           broken, potentially stopping the whole system. For example, inability
     *           to connect to a crucial database.
     *
     * TEST    - Used for experimental features and test-related logging. Helps in
     *           isolating test output from regular application logs and debugging
     *           test-specific behaviors.
     *
     * Usage of log levels should be consistent to ensure effective log management,
     * alerting, and troubleshooting.
     */
    enum LogLevel
    {
        FATAL = 0, ///< Critical issue: key business functionalities are not working.
        ERROR = 1, ///< One or more functionalities are not working properly.
        WARN = 2, ///< Unexpected behavior occurred, but the application continues.
        INFO = 3, ///< Informational message about normal application events.
        DEBUG = 4, ///< Useful for debugging and troubleshooting.
        TRACE = 5, ///< Fine-grained, highly detailed information for step-by-step tracing.
        TEST = 100, ///< Used for experimental features and test-related logging.
    };

    inline LogLevel max_log_level = TEST;

    inline std::string log_level_to_string(const LogLevel& log_level)
    {
        switch (log_level)
        {
        case FATAL: return "FATAL";
        case ERROR: return "ERROR";
        case WARN: return "WARN";
        case INFO: return "INFO";
        case DEBUG: return "DEBUG";
        case TRACE: return "TRACE";
        case TEST: return "TEST";
        default: throw std::runtime_error("Unknown log level: " + std::to_string(log_level));
        }
    }

    inline ConsoleColor log_level_to_console_color(const LogLevel& log_level)
    {
        switch (log_level)
        {
        case FATAL:
        case ERROR: return ConsoleColor::RED;
        case WARN: return ConsoleColor::YELLOW;
        case INFO: return ConsoleColor::GREEN;
        case DEBUG: return ConsoleColor::BLUE;
        case TRACE: return ConsoleColor::PURPLE;
        case TEST: return ConsoleColor::CYAN;
        default: throw std::runtime_error("Unknown log level: " + std::to_string(log_level));
        }
    }

    class Logger : public ConsolePrinter
    {
    private:
        LogLevel level;

    public:
        Logger(LogLevel log_level)
            : ConsolePrinter(
                  "[" + log_level_to_string(log_level) + "] ",
                  "",
                  log_level <= max_log_level,
                  log_level_to_console_color(log_level)
              ),
              level(log_level)
        {
        }
    };
}
#endif // LOGGER_H
