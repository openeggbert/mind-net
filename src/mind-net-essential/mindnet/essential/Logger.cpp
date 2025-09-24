#include "mindnet/essential/Logger.h"

#include "mindnet/essential/ConsolePrinter.h"
#include "mindnet/essential/Configuration.h"

namespace mindnet::essential {
    Logger::Logger(LogLevel log_level)
        : ConsolePrinter(
              "[" + log_level_to_string(log_level) + "] ",
              "",
              log_level_to_console_color(log_level)
          ),
          level(log_level)
    {
    }

    bool Logger::is_enabled()
    {
        return level <= g_configuration.max_log_level;
    }
}
