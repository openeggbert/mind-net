//
// Created by robertvokac on 9/5/25.
//

#ifndef MIND_NET_CONFIGURATION_H
#define MIND_NET_CONFIGURATION_H

#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <map>

#include "enums/UserRole.h"

namespace mindnet
{
    inline string_map loadProperties(const std::string& filename)
    {
        string_map properties;
        std::ifstream file(filename);
        if (!file)
        {
            std::cerr << "Failed to open file: " << filename << "\n";
            return properties;
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
        bool allow_public_access{true};
        bool allow_self_registration{true};
        bool require_admin_approval_for_new_users{false};
        enums::UserRole default_user_role{enums::UserRole::READER};
        std::string jwt_secret{};

        bool string_to_bool(std::string& str)
        {
            if (str == "false") return false;
            if (str == "true") return true;
            throw std::runtime_error("Invalid boolean value: " + str);
        }

        bool map_contains(string_map& map, std::string key)
        {
            return map.find(key) != map.end();
        }

        Configuration(string_map map)
        {
            if (map_contains(map, "allow_public_access"))
                allow_public_access = string_to_bool(
                    map["allow_public_access"]);
            if (map_contains(map, "allow_self_registration"))
                allow_self_registration = string_to_bool(
                    map["allow_self_registration"]);
            if (map_contains(map, "require_admin_approval_for_new_users"))
                require_admin_approval_for_new_users = string_to_bool(
                    map["require_admin_approval_for_new_users"]);
            if (map_contains(map, "default_user_role")) default_user_role = enums::string_to_user_role(map["default_user_role"]);
            if (map_contains(map, "jwt_secret")) jwt_secret = map["jwt_secret"];
        }
    };

    inline const Configuration g_configuration(loadProperties("./mindnet.properties"));
}


#endif //MIND_NET_CONFIGURATION_H
