//
// Created by robertvokac on 7/31/25.
//

#ifndef HELPER_H
#define HELPER_H
#include <string>

#include "enums/ColumnType.h"
#include "nlohmann/json_fwd.hpp"

#define cast64 static_cast<int64_t>

typedef long long ll;
typedef unsigned long long ull;
typedef ull unixtime;
typedef unsigned short ushort;
typedef std::string str;
typedef nlohmann::json JSON;
typedef std::variant<std::string, int64_t> entity_field;
typedef std::vector<entity_field> entity_fields;
using std::endl;

struct ColumnDefinition
{
    str column_name;
    mindnet::enums::ColumnType column_type;
    bool mandatory = true;
};
typedef ColumnDefinition column_definition;
typedef std::vector<column_definition> column_definitions;

static constexpr const char* PRIMARY_KEY_COLUMN_NAME = "id";
static constexpr const char* FOREIGN_KEY_NULL = "FOREIGN_KEY_NULL";

#endif //HELPER_H
