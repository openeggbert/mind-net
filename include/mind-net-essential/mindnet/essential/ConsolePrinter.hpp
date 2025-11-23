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
