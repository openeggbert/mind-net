//
// Created by robertvokac on 7/31/25.
//

#pragma once

#include <map>
#include <string>
#include <variant>
#include <vector>

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
typedef int64_t i64;

typedef std::variant<std::string, int64_t> entity_field;
typedef std::vector<entity_field> entity_fields;

typedef std::map<std::string, std::string> string_map;

static constexpr const char* PRIMARY_KEY_COLUMN_NAME = "id";

