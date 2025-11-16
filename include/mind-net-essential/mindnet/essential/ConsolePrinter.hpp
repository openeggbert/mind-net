#pragma once
#include <iostream>
#include <sstream>
#include <string>
#include <mutex>

#include "ConsoleColor.hpp"

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
        mutable std::recursive_mutex mtx;

    public:
        ConsoleColor color = ConsoleColor::UNKNOWN;
        print_timestamp_function print_timestamp_function_pointer = nullptr;

        ConsolePrinter(
            std::string before = "",
            std::string after = "",
            ConsoleColor color_ = ConsoleColor::UNKNOWN,
            print_timestamp_function print_timestamp_function_pointer_ = nullptr
        );

        virtual bool is_enabled()
        {
            return true;
        }

        bool is_disabled()
        {
            return !is_enabled();
        }

        template <typename T>
        ConsolePrinter& operator<<(const T& value)
        {
            if (is_enabled())
            {
                std::lock_guard<std::recursive_mutex> lock(mtx);
                buffer << value;
            }
            return *this;
        }

        ConsolePrinter& operator<<(ConsolePrinter& (*manip)(ConsolePrinter&));
        ConsolePrinter& operator<<(std::ostream& (*manip)(std::ostream&));

        void set_timestamp_function(print_timestamp_function fn);

        void flush(bool new_line);
        ~ConsolePrinter();
    };
}
