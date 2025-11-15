#include "mindnet/essential/ConsolePrinter.hpp"

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
        ConsoleColor color_,
        print_timestamp_function print_timestamp_function_pointer_
    )
        : prefix(std::move(before)),
          suffix(std::move(after)),
          color(color_),
          print_timestamp_function_pointer(print_timestamp_function_pointer_)
    {
    }

    ConsolePrinter& ConsolePrinter::operator<<(ConsolePrinter& (*manip)(ConsolePrinter&))
    {
        std::lock_guard<std::recursive_mutex> lock(mtx);
        return manip(*this);
    }

    ConsolePrinter& ConsolePrinter::operator<<(std::ostream& (*manip)(std::ostream&))
    {
        std::lock_guard<std::recursive_mutex> lock(mtx);

        if (is_disabled()) return *this;
        if (manip == static_cast<std::ostream& (*)(std::ostream&)>(std::endl))
        {
            flush(false);

            ConsoleColor old_color = color;
            color = ConsoleColor::RED;
            std::cout << "\n[WARN] ConsolePrinter: !!!endl used instead of commit: " + last_buffer_str;
            color = old_color;

            manip(std::cout); // prints '\n' and flush
        }
        else
        {
            manip(std::cout);
        }
        return *this;
    }

    void ConsolePrinter::set_timestamp_function(print_timestamp_function fn)
    {
        std::lock_guard<std::recursive_mutex> lock(mtx);
        print_timestamp_function_pointer = fn;
    }

    void ConsolePrinter::flush(bool new_line)
    {
        std::lock_guard<std::recursive_mutex> lock(mtx);

        if (is_disabled()) return;

        if (color != ConsoleColor::UNKNOWN)
            std::cout << "\033[" << static_cast<int>(color) << "m";

        if (print_timestamp_function_pointer)
            std::cout << print_timestamp_function_pointer() << " ";

        std::string current = buffer.str();
        std::cout << prefix << current << suffix;

        if (color != ConsoleColor::UNKNOWN)
            std::cout << "\033[0m";

        if (new_line)
            std::cout << std::endl;

        last_buffer_str = std::move(current);

        std::ostringstream().swap(buffer); // vyčistí buffer
    }

    ConsolePrinter::~ConsolePrinter()
    {
        if (is_enabled() && !buffer.str().empty())
            flush(true); // flush si mutex zamkne samo
    }
}
