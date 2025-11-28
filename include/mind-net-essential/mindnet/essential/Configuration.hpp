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

#include <fstream>
#include <map>
#include <fmt/core.h>
#include <fmt/format.h>
#include <fmt/args.h>

#include "AccessMode.hpp"
#include "DatabaseType.hpp"
#include "Environment.hpp"
#include "RegistrationMode.hpp"
#include "Helper.hpp"
#include "LogLevel.hpp"

// How to add new configuration entry:
// - Modify also the file "SuperAdminEndpointsGenerator.cpp"

namespace mindnet::essential
{
    string_map load_mind_net_properties(const std::string& filename);

    // Access token expiration (in minutes)
    constexpr int ACCESS_TOKEN_EXPIRES_IN_MIN_VALUE = 5; // 5 minutes
    constexpr int ACCESS_TOKEN_EXPIRES_IN_MAX_VALUE = 43200; // 30 days

    // Refresh token expiration (in minutes)
    constexpr int REFRESH_TOKEN_EXPIRES_IN_MIN_VALUE = 1440; // 1 day
    constexpr int REFRESH_TOKEN_EXPIRES_IN_MAX_VALUE = 432000; // ~300 days

    // Refresh token rotation threshold (in minutes)
    constexpr int REFRESH_TOKEN_ROTATION_THRESHOLD_MIN_VALUE = 60; // 1 hour
    // max threshold = refresh_token_expires_in (will be checked dynamically)
    // therefore we only set an "absolute upper bound" here
    constexpr int REFRESH_TOKEN_ROTATION_THRESHOLD_MAX_VALUE = REFRESH_TOKEN_EXPIRES_IN_MAX_VALUE;

    struct Configuration
    {
        //identification
        std::string name;
        std::string description;
        Environment environment{Environment::Production};
        mutable unsigned short port{};
        mutable unsigned short frontend_port{};
        mutable std::string host{};
        DatabaseType database_type{DatabaseType::Unknown};
        //access
        AccessMode access_mode{AccessMode::PublicFullAccess};
        RegistrationMode registration_mode{RegistrationMode::Free};
        UserRole default_user_role{UserRole::Reader};
        //database
        std::string db_host;
        int db_port{};
        std::string db_name;
        std::string db_user;
        std::string db_password;
        //other
        LogLevel max_log_level{ERROR};
        std::set<string> allowed_plugins;
        i64 access_token_expires_in{15}; // minutes
        i64 refresh_token_expires_in{43200}; // minutes (30 days)
        i64 refresh_token_rotation_threshold_in{10080}; // minutes (7 days)
        i64 read_cache_capacity_size{10000};
        size_t read_cache_capacity_bytes{1073741824};

        Configuration(const string_map& map);
        bool save_mind_net_properties();
        fmt::dynamic_format_arg_store<fmt::format_context> to_fmt_store();
        std::string validate();
    };

    inline Configuration g_configuration(load_mind_net_properties("./mindnet.properties"));
}