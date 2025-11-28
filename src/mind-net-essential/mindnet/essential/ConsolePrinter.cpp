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
