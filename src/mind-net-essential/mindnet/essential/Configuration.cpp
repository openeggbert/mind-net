//
// Created by robertvokac on 9/5/25.
//

#include "mindnet/essential/Configuration.h"

#include <filesystem>
#include <iostream>
#include <sstream>
#include <fmt/core.h>
#include <fmt/format.h>

#include "mindnet/essential/Global.h"

#define if_map_has(key) if (map_contains(map, #key))
#define save_enum(key) if_map_has(key) key = string_to_##key(map.at( #key));
#define save_text(key) if_map_has(key) key = map.at( #key);
#define save_number(key) if_map_has(key) key = std::stoi(map.at( #key));

namespace mindnet::essential
{
    string_map load_mind_net_properties(const std::string& filename)
    {
        string_map properties;
        std::ifstream file(filename);
        if (!file)
        {
            fatal << "Failed to open file: " << filename
                << "\nCurrent working dir: " << std::filesystem::current_path() << commit;
            exit(1);
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

    std::set<string> split_string_by_commas(const string& string_, std::set<std::string>& result)
    {
        if (!string_.empty())
        {
            std::stringstream ss(string_);
            std::string field_entry;

            while (std::getline(ss, field_entry, ','))
            {
                result.insert(field_entry);
            }
        }
        return result;
    }

    string join_strings_by_commas(std::set<std::string>& strings)
    {
        if (strings.empty())
        {
            return "";
        }

        std::stringstream ss;
        auto it = strings.begin();
        ss << *it;
        ++it;

        while (it != strings.end())
        {
            ss << "," << *it;
            ++it;
        }
    
        return ss.str();
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
        using mindnet::essential::string_to_database_type;
        save_enum(database_type)
        //
        save_enum(access_mode)
        save_enum(registration_mode)
        if_map_has(default_user_role)
            default_user_role = string_to_user_role(
                map.at("default_user_role"));
        //
        save_text(jwt_secret)
        //
        if (map_contains(map, "max_log_level")) max_log_level = string_to_log_level(map.at("max_log_level"));
        if (map_contains(map, "allowed_plugins"))
        {
            auto& allowed_plugins_string = map.at("allowed_plugins");
            split_string_by_commas(allowed_plugins_string, allowed_plugins);
        }
    }

    constexpr auto mind_net_properties_template = FMT_STRING(R"(
#Identification
name={name}
description={description}
environment={environment}

#Environment
host={host}
port={port}
frontend_port={frontend_port}
database_type={database_type}

#Access
access_mode={access_mode}
registration_mode={registration_mode}
default_user_role={default_user_role}

#Secrets
jwt_secret={jwt_secret}

#Other
max_log_level={max_log_level}
allowed_plugins={allowed_plugins}

)");

    bool Configuration::save_mind_net_properties()
    {
        {
            namespace fs = std::filesystem;

            string backup_dir = "mindnet.properties.backups";

            if (!fs::exists(backup_dir))
            {
                try
                {
                    fs::create_directory(backup_dir);
                    info << "Directory created: " << backup_dir << commit;
                }
                catch (const fs::filesystem_error& e)
                {
                    fatal << "Error during creating directory: " << backup_dir << " " << e.what() << commit;
                    return false;;
                }
            }

            string old_name = "mindnet.properties";
            auto new_name = backup_dir + "/" + old_name + std::to_string(static_cast<long long>(std::time(nullptr)));
            try
            {
                fs::rename(old_name, new_name);
                info << "File " << old_name << " renamed successfully to " << new_name << commit;
            }
            catch (const fs::filesystem_error& e)
            {
                fatal << "Renaming file failed: old_name=" << old_name <<
                ", new_name=" << new_name << ", reason=" << e.what() << commit;
                return false;
            }
            {
                const fs::path dir = backup_dir;

                for (const auto& entry : fs::directory_iterator(dir)) {
                    if (fs::is_regular_file(entry.path())) {
                        auto ftime = fs::last_write_time(entry);

                        // Convert filesystem time to system_clock::time_point
                        auto sctp = std::chrono::clock_cast<std::chrono::system_clock>(ftime);

                        auto now = std::chrono::system_clock::now();
                        auto age = now - sctp;

                        // Convert to full days
                        auto days = std::chrono::duration_cast<std::chrono::days>(age).count();

                        debug << entry.path().filename().string() << " is " << days << " days old" << commit;

                        if (days > 365) {
                            info << "Deleting " << entry.path() <<
                                ", which is old " << days << " days " << commit;

                            //todo remove macro
                            #define enable_backup_cleanup
                            #ifdef enable_backup_cleanup
                            std::error_code ec;
                            fs::remove(entry.path(), ec);
                            if (ec) {
                                warn << "Failed to delete " << entry.path()
                                          << ": " << ec.message() << commit;
                            }
                            #endif
                        }

                    }
                }
            }
        }

        std::string mind_net_properties = fmt::format(
            mind_net_properties_template,
            fmt::arg("name", name),
            fmt::arg("description", description),
            fmt::arg("environment", environment_to_string(environment)),
            //
            fmt::arg("host", host),
            fmt::arg("port", port),
            fmt::arg("frontend_port", frontend_port),
            fmt::arg("database_type", database_type_to_string(database_type)),
            //
            fmt::arg("access_mode", access_mode_to_string(access_mode)),
            fmt::arg("registration_mode", registration_mode_to_string(registration_mode)),
            fmt::arg("default_user_role", user_role_to_string(default_user_role)),
            //
            fmt::arg("jwt_secret", jwt_secret),
            //
            fmt::arg("max_log_level", log_level_to_string(max_log_level)),
            fmt::arg("allowed_plugins", join_strings_by_commas(allowed_plugins))
        );

        // save to file
        std::ofstream file("mindnet.properties"); // opens file for writing
        if (!file)
        {
            throw std::runtime_error("Failed to create file");
        }
        file << mind_net_properties; // writes string
        // file.close(); // not necessary, closes automatically when ofstream is destroyed

        return true;
    }
}
