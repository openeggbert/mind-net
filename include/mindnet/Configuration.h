//
// Created by robertvokac on 9/5/25.
//

#ifndef MIND_NET_CONFIGURATION_H
#define MIND_NET_CONFIGURATION_H

#include <fstream>
#include <iostream>
#include <map>

#include "AccessMode.h"
#include "DatabaseType.h"
#include "Environment.h"
#include "RegistrationMode.h"
#include "api/MigrationScripts.h"
#include "plugins/core/enums/UserRole.h"
#define if_map_has(key) if (map_contains(map, #key))
#define save_enum(key) if_map_has(key) key = string_to_##key(map[ #key]);
#define save_text(key) if_map_has(key) key = map[ #key];
#define save_number(key) if_map_has(key) key = std::stoi(map[ #key]);
#define MIND_NET_VERSION_MAYOR 0
#define MIND_NET_VERSION_MINOR 0
#define MIND_NET_VERSION_PATCH 0
#define MIND_NET_VERSION_LABEL snapshot
#define MIND_NET_VERSION MIND_NET_VERSION_MAYOR.MIND_NET_VERSION_MINOR.MIND_NET_VERSION_PATCH-MIND_NET_VERSION_LABEL
#define MIND_NET_BUILD_TIME std::string(__DATE__) + " " + __TIME__
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

namespace mindnet
{
    inline string_map loadProperties(const std::string& filename)
    {
        string_map properties;
        std::ifstream file(filename);
        if (!file)
        {
            std::cerr << "Failed to open file: " << filename << "\n";
            exit(1);
            //return properties;
        }

        std::string line;
        while (std::getline(file, line))
        {
            line.erase(0, line.find_first_not_of(" \t\r\n"));
            line.erase(line.find_last_not_of(" \t\r\n") + 1);

            if (line.empty() || line[0] == '#' || line[0] == '!')
                continue;

            size_t eqPos = line.find('=');
            if (eqPos == std::string::npos)
                continue;

            std::string key = line.substr(0, eqPos);
            std::string value = line.substr(eqPos + 1);

            key.erase(0, key.find_first_not_of(" \t\r\n"));
            key.erase(key.find_last_not_of(" \t\r\n") + 1);
            value.erase(0, value.find_first_not_of(" \t\r\n"));
            value.erase(value.find_last_not_of(" \t\r\n") + 1);

            properties[key] = value;
        }

        return properties;
    }

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
        plugins::core::enums::UserRole default_user_role{plugins::core::enums::UserRole::Reader};
        //secrets
        std::string jwt_secret{};

        bool string_to_bool(std::string& str)
        {
            if (str == "false") return false;
            if (str == "true") return true;
            throw std::runtime_error("Invalid boolean value: " + str);
        }

        bool map_contains(string_map& map, const std::string& key)
        {
            return map.find(key) != map.end();
        }

        Configuration(string_map map)
        {
            save_text(name)
            save_text(description)
            save_enum(environment)
            //
            save_number(port)
            save_number(frontend_port)
            save_text(host)
            using mindnet::string_to_database_type;
            save_enum(database_type)
            //
            save_enum(access_mode)
            save_enum(registration_mode)
            if_map_has(default_user_role) default_user_role = plugins::core::enums::string_to_user_role(map[ "default_user_role"]);
            //
            save_text(jwt_secret)
            //

        }
    };

    inline const Configuration g_configuration(loadProperties("./mindnet.properties"));
}

#endif //MIND_NET_CONFIGURATION_H
