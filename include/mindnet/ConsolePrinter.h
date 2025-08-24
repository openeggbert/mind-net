#ifndef CONSOLEPRINTER_H
#define CONSOLEPRINTER_H
#include <iostream>
#include <sstream>
#include <string>
#include <utility>

namespace mindnet
{
    typedef std::string (*print_timestamp_function)();

    class ConsolePrinter; // fwd decl

    ConsolePrinter& commit(ConsolePrinter& printer);

    enum class ConsoleColor
    {
        UNKNOWN = -1,
        BLACK = 30,
        RED = 31,
        GREEN = 32,
        YELLOW = 33,
        BLUE = 34,
        PURPLE = 35,
        CYAN = 36,
        WHITE = 37
    };

    class ConsolePrinter
    {
        std::ostringstream buffer;
        std::string prefix;
        std::string suffix;

    public:
        ConsoleColor color = ConsoleColor::UNKNOWN;
        bool enabled = true;
        print_timestamp_function print_timestamp_function_pointer = nullptr;

        ConsolePrinter(
            std::string before = "",
            std::string after = "",
            bool enabled_ = true,
            ConsoleColor color_ = ConsoleColor::UNKNOWN,
            print_timestamp_function print_timestamp_function_pointer_ = nullptr
        );

        template <typename T>
        ConsolePrinter& operator<<(const T& value)
        {
            if (enabled) buffer << value;
            return *this;
        }

        ConsolePrinter& operator<<(ConsolePrinter& (*manip)(ConsolePrinter&))
        {
            return manip(*this);
        }

        ConsolePrinter& operator<<(std::ostream& (*manip)(std::ostream&))
        {
            if (manip == static_cast<std::ostream& (*)(std::ostream&)>(std::endl))
            {
                flush(false);
            }
            return *this;
        }

        void set_timestamp_function(print_timestamp_function fn);

        void flush(bool new_line);
        ~ConsolePrinter();
    };


}
#endif // CONSOLEPRINTER_H
