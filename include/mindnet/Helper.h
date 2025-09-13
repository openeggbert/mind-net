//
// Created by robertvokac on 7/31/25.
//

#ifndef HELPER_H
#define HELPER_H
#include <string>

#include "models/misc/ColumnDefinition.h"
#include "nlohmann/json_fwd.hpp"

#define cast64 static_cast<int64_t>
#define castint static_cast<int>
#define PASTE(a,b) a##b
#define XPASTE(a,b) PASTE(a,b)
#define STRINGIFY_HELPER(x) #x
#define STRINGIFY(x) STRINGIFY_HELPER(x)

typedef long long ll;
typedef unsigned long long ull;
typedef ull unixtime;
typedef unsigned short ushort;
using std::string;
typedef nlohmann::json JSON;
typedef std::variant<std::string, int64_t> entity_field;
typedef std::vector<entity_field> entity_fields;
using std::endl;
typedef std::map<std::string, std::string> string_map;

typedef mindnet::models::misc::ColumnDefinition column_definition;
typedef std::vector<column_definition> column_definitions;

static constexpr const char* PRIMARY_KEY_COLUMN_NAME = "id";
static constexpr const char* FOREIGN_KEY_NULL = "FOREIGN_KEY_NULL";

#endif //HELPER_H
