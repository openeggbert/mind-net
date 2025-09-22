#include "../../../include/mindnet/core/ConsolePrinter.h"
#include <stdexcept>
#include "../../../include/mindnet/Utils.h"

namespace mindnet::core
{

    Logger::Logger(LogLevel log_level)
        : ConsolePrinter(
              "[" + log_level_to_string(log_level) + "] ",
              "",
              log_level <= max_log_level,
              log_level_to_console_color(log_level)
          ),
          level(log_level)
    {
    }
}
