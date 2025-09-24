//
// Created by robertvokac on 8/8/25.
//

#include "mindnet/essential/ConsolePrinter.h"

namespace mindnet::essential
{
    ConsolePrinter& commit(ConsolePrinter& printer)
    {
        printer.flush(true);
        return printer;
    }

    ConsolePrinter::ConsolePrinter(
        std::string before,
        std::string after,
        bool_predicate enabled_predicate_,
        ConsoleColor color_,
        print_timestamp_function print_timestamp_function_pointer_
    )
        : prefix(std::move(before)),
          suffix(std::move(after)),
          enabled_predicate(enabled_predicate_ ? enabled_predicate_ : [] { return true; }),
          color(color_),
          print_timestamp_function_pointer(print_timestamp_function_pointer_)
    {
        if (!enabled_predicate)
        {
            throw std::runtime_error("You set empty enabled_predicate");
        }
    }

    ConsolePrinter& ConsolePrinter::operator<<(ConsolePrinter& (*manip)(ConsolePrinter&))
    {
        return manip(*this);
    }

    ConsolePrinter& ConsolePrinter::operator<<(std::ostream& (*manip)(std::ostream&))
    {
        if (!enabled_predicate) std::cerr <<"Handler enabled_predicate not set : ConsolePrinter& ConsolePrinter::operator<<(std::ostream& (*manip)(std::ostream&))" << std::flush;
        if (enabled_predicate && !enabled_predicate()) return *this;
        if (manip == static_cast<std::ostream& (*)(std::ostream&)>(std::endl))
        {
            // 1. Flush current buffer
            flush(false);

            // 2. Print warning in red on a new line
            ConsoleColor old_color = color;
            color = ConsoleColor::RED;
            std::cout << "\nConsolePrinter: !!!endl used instead of commit: " + last_buffer_str;
            color = old_color;

            // 3. Standard endl
            manip(std::cout); // prints '\n' and flush
        }
        else
        {
            manip(std::cout); // other manipulator
        }
        return *this;
    }

    void ConsolePrinter::set_timestamp_function(print_timestamp_function fn)
    {
        print_timestamp_function_pointer = fn;
    }

    void ConsolePrinter::flush(bool new_line)
    {
        if (!enabled_predicate) std::cerr <<"Handler enabled_predicate not set : flush(bool new_line)" << std::flush;
        if (!enabled_predicate()) return;
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
        if (!enabled_predicate) std::cerr <<"Handler enabled_predicate not set:ConsolePrinter::~ConsolePrinter()" << std::flush;

        if (enabled_predicate() && !buffer.str().empty()) flush(true);
    }
}
