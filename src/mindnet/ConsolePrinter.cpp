//
// Created by robertvokac on 8/8/25.
//

#include "mindnet/ConsolePrinter.h"

namespace mindnet
{
    ConsolePrinter& commit(ConsolePrinter& printer)
    {
        printer.flush(true);
        return printer;
    }

    ConsolePrinter::ConsolePrinter(
        std::string before,
        std::string after,
        bool enabled_,
        ConsoleColor color_,
        print_timestamp_function print_timestamp_function_pointer_
    )
        : prefix(std::move(before)),
          suffix(std::move(after)),
          enabled(enabled_),
          color(color_),
          print_timestamp_function_pointer(print_timestamp_function_pointer_)
    {
    }


    void ConsolePrinter::set_timestamp_function(print_timestamp_function fn)
    {
        print_timestamp_function_pointer = fn;
    }

    void ConsolePrinter::flush(bool new_line)
    {
        std::cout << (color != ConsoleColor::UNKNOWN ? "\033[" + std::to_string(static_cast<int>(color)) + "m" : "");
        if (print_timestamp_function_pointer) std::cout << print_timestamp_function_pointer() << " ";
        std::cout << prefix << buffer.str() << suffix;
        if (color != ConsoleColor::UNKNOWN) std::cout << "\033[0m";
        if (new_line) { std::cout << std::endl; }

        last_buffer_str = buffer.str();
        buffer.str("");
        buffer.clear();
    }

    ConsolePrinter::~ConsolePrinter()
    {
        if (enabled && !buffer.str().empty()) flush(true);
    }
}
