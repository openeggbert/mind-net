///////////////////////////////////////////////////////////////////////////////////////////////
// mind-net : Mind map software.
// Copyright (C) 2025-2025 the original author or authors.
//
// This program is free software: you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation, either version 3
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program. If not, see
// <https://www.gnu.org/licenses/> or write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
///////////////////////////////////////////////////////////////////////////////////////////////
#ifndef ENUMDEFINITION_H
#define ENUMDEFINITION_H


#include <cstdarg>
#include <stdexcept>
#include <string>

namespace mindnet::models::misc
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

        short get_value_count() const
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

        short get_value(const size_t index) const
        {
            check_index_is_valid(index);
            return values[index];
        }

        string get_value_as_string(const size_t index) const
        {
            check_index_is_valid(index);
            return enum_to_string_pointer_function(values[index]);
        }
    };
}
#endif // ENUMDEFINITION_H