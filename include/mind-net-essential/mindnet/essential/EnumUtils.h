//
// Created by robertvokac on 9/24/25.
//

#ifndef MIND_NET_ENUMUTILS_H
#define MIND_NET_ENUMUTILS_H

// EnumUtils.h
#pragma once
#include <string>
#include <vector>
#include <stdexcept>

// Generator: enum, to_string, from_string, values()
#define DECLARE_ENUM(NAME, LIST)                                            \
enum class NAME {                                                       \
LIST(DECLARE_ENUMERATOR)                                            \
};                                                                      \
\
inline std::string NAME##_to_string(NAME value) {                       \
switch (value) {                                                    \
LIST(DECLARE_ENUM_TO_STRING_CASE)                               \
default: return "Unknown";                                          \
}                                                                   \
}                                                                       \
\
inline std::vector<NAME> NAME##_to_values() {                           \
return { LIST(DECLARE_ENUM_TO_VALUES) };                            \
}                                                                       \
\
inline NAME string_to_##NAME(const std::string& str) {                  \
LIST(DECLARE_STRING_TO_ENUM_IF)                                     \
throw std::runtime_error("Invalid " #NAME ": " + str);              \
}

// Helper macros
#define DECLARE_ENUMERATOR(name, value) name = value,
#define DECLARE_ENUM_TO_STRING_CASE(name, value) case NAME::name: return #name;
#define DECLARE_ENUM_TO_VALUES(name, value) NAME::name,
#define DECLARE_STRING_TO_ENUM_IF(name, value) if (str == #name) return NAME::name;

#endif //MIND_NET_ENUMUTILS_H

//Usage

// // UserRole.h
// #ifndef USERROLE_H
// #define USERROLE_H
//
// #include "EnumUtils.h"
//
// namespace mindnet::essential {
//
//     // Definitions of lists using X-macros
// #define USERROLE_LIST(X) \
// X(Guest, 0)          \
// X(Reader, 1)         \
// X(Editor, 2)         \
// X(Reviewer, 3)       \
// X(Admin, 4)          \
// X(SuperAdmin, 5)     \
// X(System, 100)
//
//     // Generate enum + funkce
//     DECLARE_ENUM(UserRole, USERROLE_LIST)
//
//     } // namespace mindnet::essential
//
// #endif // USERROLE_H
//
