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



#include <cstdarg>
#include <stdexcept>
#include <string>


namespace mindnet::model
{
    typedef std::string (*enum_to_string_pointer)(int);
    using std::string;

    /**
     * Represents the definition of a database model including its name,
     * auto-increment status, columns and allowed CRUD operations.
     */
    struct EnumDefinition
    {
    private:
        short* values = nullptr;
        short value_count;
        enum_to_string_pointer enum_to_string_pointer_function;

    public:
        /**
         * Constructs a ModelDefinition with the given name
         * @param name The name for the model
         */

        EnumDefinition(
            enum_to_string_pointer enum_to_string_pointer_function_,
            int count,
            ...
        )
            :
            enum_to_string_pointer_function(enum_to_string_pointer_function_),
            value_count(count)
        {
            values = new short[count];
            va_list args;
            va_start(args, count);

            for (int i = 0; i < count; ++i)
            {
                values[i] = va_arg(args, int);
            }

            va_end(args);
        };

        EnumDefinition(const EnumDefinition& other)
            : value_count(other.value_count),
              enum_to_string_pointer_function(other.enum_to_string_pointer_function)
        {
            values = new short[value_count];
            std::copy(other.values, other.values + value_count, values);
        }

        EnumDefinition& operator=(const EnumDefinition& other)
        {
            if (this != &other)
            {
                delete[] values;
                value_count = other.value_count;
                enum_to_string_pointer_function = other.enum_to_string_pointer_function;
                values = new short[value_count];
                std::copy(other.values, other.values + value_count, values);
            }
            return *this;
        }

        ~EnumDefinition()
        {
            if (values != nullptr)
            {
                delete[] values;
            }
        }

        [[nodiscard]] short get_value_count() const
        {
            return value_count;
        }

        bool is_index_valid(const int index) const
        {
            return index >= 0 && index < value_count;
        }

        void check_index_is_valid(const size_t index) const
        {
            if (!is_index_valid(index))
            {
                throw std::out_of_range("EnumDefinition: index out of range");
            }
        }

        [[nodiscard]] short get_value(const size_t index) const
        {
            check_index_is_valid(index);
            return values[index];
        }

        string get_value_as_string(const size_t index) const
        {
            check_index_is_valid(index);
            return enum_to_string_pointer_function(values[index]);
        }

        bool is_value_valid(const short value) const
        {
            for (int i = 0; i < value_count; ++i)
            {
                if (values[i] == value)
                {
                    return true;
                }
            }
            return false;
        }
    };
}
