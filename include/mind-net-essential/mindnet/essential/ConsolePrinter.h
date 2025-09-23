#ifndef CONSOLEPRINTER_H
#define CONSOLEPRINTER_H
#include <iostream>
#include <sstream>
#include <string>

#include "ConsoleColor.h"

namespace mindnet::essential
{
    using print_timestamp_function = std::string (*)();

    class ConsolePrinter; // fwd decl

    ConsolePrinter& commit(ConsolePrinter& printer);

    class ConsolePrinter
    {
        std::ostringstream buffer;
        std::string prefix;
        std::string suffix;
        std::string last_buffer_str;

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

        ConsolePrinter& operator<<(ConsolePrinter& (*manip)(ConsolePrinter&));

        ConsolePrinter& operator<<(std::ostream& (*manip)(std::ostream&));

        void set_timestamp_function(print_timestamp_function fn);

        void flush(bool new_line);
        ~ConsolePrinter();
    };
}
#endif // CONSOLEPRINTER_H
