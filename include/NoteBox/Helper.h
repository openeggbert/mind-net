//
// Created by robertvokac on 7/31/25.
//

#ifndef HELPER_H
#define HELPER_H
#include <string>

#include "nlohmann/json_fwd.hpp"

typedef long long ll;
typedef unsigned long long ull;
typedef ull unixtime;
typedef unsigned short ushort;
typedef std::string str;
typedef nlohmann::json JSON;
typedef std::variant<std::string, int64_t> entity_field;
typedef std::vector<entity_field> entity_fields;
typedef std::vector<const char*> entity_columns;

#endif //HELPER_H
