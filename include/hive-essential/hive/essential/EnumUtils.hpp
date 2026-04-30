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

#include <string>
#include <vector>
#include <stdexcept>
#include "hive/model/EnumDefinition.hpp"

// NAME   = name of the enum (e.g. RepetitionScope)
// PREFIX = prefix for functions (e.g. repetition_scope)
#define DECLARE_ENUM(NAME, PREFIX, LIST)                                     \
enum class NAME {                                                            \
    LIST(DECLARE_ENUMERATOR, NAME)                                           \
};                                                                           \
                                                                             \
inline std::string PREFIX##_to_string(NAME value) {                          \
    switch (value) {                                                         \
        LIST(DECLARE_ENUM_TO_STRING_CASE, NAME)                              \
        default: return "Unknown";                                           \
    }                                                                        \
}                                                                            \
                                                                             \
inline std::vector<NAME> PREFIX##_to_values() {                              \
    return { LIST(DECLARE_ENUM_TO_VALUES, NAME) };                           \
}                                                                            \
                                                                             \
inline NAME string_to_##PREFIX(const std::string& str) {                     \
    LIST(DECLARE_STRING_TO_ENUM_IF, NAME)                                    \
    throw std::runtime_error("Invalid " #NAME ": " + str);                   \
}                                                                            \
                                                                             \
inline bool PREFIX##_is_valid(const std::string& str) {                      \
    LIST(DECLARE_STRING_TO_ENUM_VALID_IF, NAME)                              \
    return false;                                                            \
}                                                                            \
                                                                             \
inline int PREFIX##_to_int(NAME value) {                                     \
    return static_cast<int>(value);                                          \
}                                                                            \
                                                                             \
inline NAME int_to_##PREFIX(int value) {                                     \
    switch (value) {                                                         \
        LIST(DECLARE_INT_TO_ENUM_CASE, NAME)                                 \
        default: throw std::runtime_error("Invalid int for " #NAME);         \
    }                                                                        \
}                                                                            \
                                                                             \
inline std::string PREFIX##_to_string(int value) {                           \
    return PREFIX##_to_string(static_cast<NAME>(value));                     \
}                                                                            \
                                                                             \
inline model::EnumDefinition PREFIX##_to_enum_definition() {                 \
    return model::EnumDefinition{ PREFIX##_to_string,                        \
        static_cast<int>(PREFIX##_to_values().size()),                       \
        LIST(DECLARE_ENUM_TO_INT_ARGS, NAME) };                              \
}

// ---------------- Helper macros ----------------
#define DECLARE_ENUMERATOR(name, value, ENUM_NAME) name = value,
#define DECLARE_ENUM_TO_STRING_CASE(name, value, ENUM_NAME) case ENUM_NAME::name: return #name;
#define DECLARE_ENUM_TO_VALUES(name, value, ENUM_NAME) ENUM_NAME::name,
#define DECLARE_STRING_TO_ENUM_IF(name, value, ENUM_NAME) if (str == #name) return ENUM_NAME::name;
#define DECLARE_STRING_TO_ENUM_VALID_IF(name, value, ENUM_NAME) if (str == #name) return true;
#define DECLARE_ENUM_TO_INT_ARGS(name, value, ENUM_NAME) value,
#define DECLARE_INT_TO_ENUM_CASE(name, value, ENUM_NAME) case value: return ENUM_NAME::name;