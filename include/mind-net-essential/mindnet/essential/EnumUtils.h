//
// Created by robertvokac on 9/24/25.
//
#ifndef MIND_NET_ENUMUTILS_H
#define MIND_NET_ENUMUTILS_H

#pragma once
#include <string>
#include <vector>
#include <stdexcept>
#include "mindnet/model/EnumDefinition.h"

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
inline std::string PREFIX##_to_string(int value) {                           \
    return PREFIX##_to_string(static_cast<NAME>(value));                     \
}                                                                            \
                                                                             \
inline NAME int_to_##PREFIX(int value) {                                     \
    switch (value) {                                                         \
        LIST(DECLARE_INT_TO_ENUM_CASE, NAME)                                 \
        default: throw std::runtime_error("Invalid int for " #NAME);         \
    }                                                                        \
}                                                                            \
                                                                             \
inline int PREFIX##_to_int(NAME value) {                                     \
    return static_cast<int>(value);                                          \
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
#define DECLARE_ENUM_TO_INT_ARGS(name, value, ENUM_NAME) value,
#define DECLARE_INT_TO_ENUM_CASE(name, value, ENUM_NAME) case value: return ENUM_NAME::name;

#endif // MIND_NET_ENUMUTILS_H


//Usage

// #ifndef REPETITION_SCOPE_H
// #define REPETITION_SCOPE_H
//
// #include <string>
// #include "mindnet/model/EnumDefinition.h"
// #include "mindnet/essential/EnumUtils.h"
//
// namespace mindnet::plugins::repetition::enums
// {
//     /**
//      *
//      * @author robertvokac
//      */
// #define REPETITION_SCOPE_LIST(X, ENUM_NAME) \
// X(DueOnly, 0, ENUM_NAME)                \
// X(NewOnly, 1, ENUM_NAME)                \
// X(DueAndNew, 2, ENUM_NAME)               \
// X(All, 3, ENUM_NAME)                 \
//
//     DECLARE_ENUM(RepetitionScope, repetition_scope, REPETITION_SCOPE_LIST)
// } // namespace mindnet::plugins::repetition::enums
// #endif


