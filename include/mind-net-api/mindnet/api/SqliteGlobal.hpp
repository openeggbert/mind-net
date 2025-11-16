//
// Created by robertvokac on 11/16/25.
//

#pragma once
#include <mutex>
#include "mindnet/essential/Global.hpp"
#include "mindnet/essential/DatabaseType.hpp"

#define SQLITE_LOCK_GUARD() \
std::unique_lock<std::mutex> lock; \
if (mindnet::essential::g_configuration.database_type == mindnet::essential::DatabaseType::SQLite) { \
lock = std::unique_lock<std::mutex>(mindnet::api::sqlite_mutex); \
}

namespace mindnet::api
{
    extern std::mutex sqlite_mutex;
}

