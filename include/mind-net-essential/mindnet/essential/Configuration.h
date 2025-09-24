//
// Created by robertvokac on 9/5/25.
//

#ifndef MIND_NET_CONFIGURATION_H
#define MIND_NET_CONFIGURATION_H

#include <fstream>
#include <map>

#include "AccessMode.h"
#include "DatabaseType.h"
#include "Environment.h"
#include "RegistrationMode.h"
#include "Helper.h"
#include "LogLevel.h"

/*
#Example:

#Identification
name=Robert Vokac
description=Zettelkasten system for Robert Vokac
environment=Development
#
#host=
#port=
#frontend_port=
database_type=SQLite

#Access
access_mode=PublicFullAccess
registration_mode=Free
default_user_role=Reader

#Secrets
jwt_secret=***
*/

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
        //secrets
        std::string jwt_secret{};
        //other
        LogLevel max_log_level{ERROR};
        std::set<string> allowed_plugins;

        Configuration(const string_map& map);
        bool save_mind_net_properties();
    };

    inline Configuration g_configuration(load_mind_net_properties("./mindnet.properties"));
}

#endif //MIND_NET_CONFIGURATION_H
