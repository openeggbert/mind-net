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

        Configuration(const string_map& map);
        bool save_mind_net_properties();
        fmt::dynamic_format_arg_store<fmt::format_context> to_fmt_store();
    };

    inline Configuration g_configuration(load_mind_net_properties("./mindnet.properties"));
}

#endif //MIND_NET_CONFIGURATION_H
