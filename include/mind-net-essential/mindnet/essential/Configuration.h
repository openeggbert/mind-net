//
// Created by robertvokac on 9/5/25.
//

#ifndef MIND_NET_CONFIGURATION_H
#define MIND_NET_CONFIGURATION_H

#include <fstream>
#include <map>
#include <fmt/core.h>
#include <fmt/format.h>
#include <fmt/args.h>

#include "AccessMode.h"
#include "DatabaseType.h"
#include "Environment.h"
#include "RegistrationMode.h"
#include "Helper.h"
#include "LogLevel.h"

namespace mindnet::essential
{
    string_map load_mind_net_properties(const std::string& filename);

    // Access token expiration (in minutes)
    constexpr int ACCESS_TOKEN_EXPIRES_IN_MIN_VALUE = 5;        // 5 minutes
    constexpr int ACCESS_TOKEN_EXPIRES_IN_MAX_VALUE = 43200;    // 30 days

    // Refresh token expiration (in minutes)
    constexpr int REFRESH_TOKEN_EXPIRES_IN_MIN_VALUE = 1440;    // 1 day
    constexpr int REFRESH_TOKEN_EXPIRES_IN_MAX_VALUE = 432000;  // ~300 days

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
        int access_token_expires_in{15};           // minutes
        int refresh_token_expires_in{43200};       // minutes (30 days)
        int refresh_token_rotation_threshold_in{10080}; // minutes (7 days)

        Configuration(const string_map& map);
        bool save_mind_net_properties();
        fmt::dynamic_format_arg_store<fmt::format_context> to_fmt_store();
        std::string validate();
    };

    inline Configuration g_configuration(load_mind_net_properties("./mindnet.properties"));
}

#endif //MIND_NET_CONFIGURATION_H
