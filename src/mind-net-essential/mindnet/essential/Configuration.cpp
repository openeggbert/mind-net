//
// Created by robertvokac on 9/5/25.
//

#include "mindnet/essential/Configuration.h"

#include <iostream>

#define if_map_has(key) if (map_contains(map, #key))
#define save_enum(key) if_map_has(key) key = string_to_##key(map.at( #key));
#define save_text(key) if_map_has(key) key = map.at( #key);
#define save_number(key) if_map_has(key) key = std::stoi(map.at( #key));

namespace mindnet::core
{
    string_map load_mind_net_properties(const std::string& filename)
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

    bool string_to_bool(const std::string& str)
    {
        if (str == "false") return false;
        if (str == "true") return true;
        throw std::runtime_error("Invalid boolean value: " + str);
    }

    bool map_contains(const string_map& map, const std::string& key)
    {
        return map.find(key) != map.end();
    }

    Configuration::Configuration(const string_map& map)
    {
        save_text(name)
        save_text(description)
        save_enum(environment)
        //
        save_number(port)
        save_number(frontend_port)
        save_text(host)
        using mindnet::core::string_to_database_type;
        save_enum(database_type)
        //
        save_enum(access_mode)
        save_enum(registration_mode)
        if_map_has(default_user_role) default_user_role = string_to_user_role(
            map.at("default_user_role"));
        //
        save_text(jwt_secret)
        //
        if (map_contains(map, "max_log_level")) max_log_level = string_to_log_level(map.at( "max_log_level"));
    }

}
