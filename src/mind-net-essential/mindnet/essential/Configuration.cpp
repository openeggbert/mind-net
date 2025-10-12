//
// Created by robertvokac on 9/5/25.
//

#include "mindnet/essential/Configuration.h"

#include <filesystem>
#include <iostream>
#include <sstream>

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
            std::cerr << "[FATAL] Failed to open file: " << filename
                << "\nCurrent working dir: " << std::filesystem::current_path() << std::endl;
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

    static inline std::string trim(const std::string& s)
    {
        auto start = s.begin();
        while (start != s.end() && std::isspace(*start))
            start++;

        auto end = s.end();
        do {
            end--;
        } while (end != start && std::isspace(*end));

        return std::string(start, end + 1);
    }

    std::set<std::string> split_string_by_commas(const std::string& str, std::set<std::string>& result)
    {
        std::stringstream ss(str);
        std::string token;

        while (std::getline(ss, token, ','))
        {
            token = trim(token);
            if (!token.empty())
                result.insert(token);
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
        if (database_type == DatabaseType::Unknown)
        {
            std::cerr << "[FATAL] You cannot set DatabaseType::Unknown in mindnet.properties. Exiting application." << std::endl;
            throw std::runtime_error("You cannot set DatabaseType::Unknown in mindnet.properties. Exiting application.");
        }
        //
        save_enum(access_mode)
        save_enum(registration_mode)
        if_map_has(default_user_role)
            default_user_role = string_to_user_role(
                map.at("default_user_role"));
        //database
        save_text(db_host)
        save_number(db_port)
        save_text(db_name)
        save_text(db_user)
        save_text(db_password)
        //other
        if (map_contains(map, "max_log_level")) max_log_level = string_to_log_level(map.at("max_log_level"));
        if (map_contains(map, "allowed_plugins"))
        {
            auto& allowed_plugins_string = map.at("allowed_plugins");
            split_string_by_commas(allowed_plugins_string, allowed_plugins);
        }
        save_number(access_token_expires_in)
        save_number(refresh_token_expires_in)
        save_number(refresh_token_rotation_threshold_in)
    }

    constexpr auto mind_net_properties_template = FMT_STRING(R"(
#Configuration for Mind Net

#Warning: This configuration is loaded only when Mind Net starts.

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

#Database
db_host={db_host}
db_port={db_port}
db_name={db_name}
db_user={db_user}
db_password={db_password}

#Other
max_log_level={max_log_level}
allowed_plugins={allowed_plugins}
access_token_expires_in={access_token_expires_in}
refresh_token_expires_in={refresh_token_expires_in}
refresh_token_rotation_threshold_in={refresh_token_rotation_threshold_in}

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
                    std::cout << "[INFO] " << "Directory created: " << backup_dir << std::endl;
                }
                catch (const fs::filesystem_error& e)
                {
                    std::cerr << "[FATAL] Error during creating directory: " << backup_dir << " " << e.what() << std::endl;
                    return false;;
                }
            }

            string old_name = "mindnet.properties";
            auto new_name = backup_dir + "/" + old_name + "." + std::to_string(static_cast<long long>(std::time(nullptr)));
            try
            {
                fs::rename(old_name, new_name);
                std::cout << "[INFO] File " << old_name << " renamed successfully to " << new_name << std::endl;
            }
            catch (const fs::filesystem_error& e)
            {
                std::cerr << "[FATAL] Renaming file failed: old_name=" << old_name <<
                ", new_name=" << new_name << ", reason=" << e.what() << std::endl;
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

                        //std::cout << "[TRACE] " << entry.path().filename().string() << " is " << days << " days old" << std::endl;

                        if (days > 90) {
                            std::cout << "[INFO] Deleting " << entry.path() <<
                                ", which is old " << days << " days " << std::endl;

                            //todo remove macro
                            #define enable_backup_cleanup
                            #ifdef enable_backup_cleanup
                            std::error_code ec;
                            fs::remove(entry.path(), ec);
                            if (ec) {
                                std::cout << "[WARN] Failed to delete " << entry.path()
                                          << ": " << ec.message() << std::endl;
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
            //database
            fmt::arg("db_host", db_host),
            fmt::arg("db_port", db_port),
            fmt::arg("db_name", db_name),
            fmt::arg("db_user", db_user),
            fmt::arg("db_password", db_password),
            //other
            fmt::arg("max_log_level", log_level_to_string(max_log_level)),
            fmt::arg("allowed_plugins", join_strings_by_commas(allowed_plugins)),
            fmt::arg("access_token_expires_in", access_token_expires_in),
            fmt::arg("refresh_token_expires_in", refresh_token_expires_in),
            fmt::arg("refresh_token_rotation_threshold_in", refresh_token_rotation_threshold_in)
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

    fmt::dynamic_format_arg_store<fmt::format_context> Configuration::to_fmt_store()
    {
        using std::string;

        fmt::dynamic_format_arg_store<fmt::format_context> store;
        auto push_entry = [&store](const string& key, const auto& value)
        {
            // trace << key << commit;
            store.push_back(fmt::arg(std::move(key).c_str(), value));
        };

#define push_enum(enum_name)\
    push_entry(STRINGIFY(enum_name), enum_name##_options);

        ////
        auto generate_options = [](auto values, auto to_string, auto current, const std::string& prefix) {
            std::string html;
            for (auto v : values) {
                std::string name = to_string(v);
                std::string selected = (v == current) ? " selected" : "";
                html += fmt::format("<option value=\"{}\"{}>{}</option>\n",
                                    name, selected, name);
            }
            return html;
        };

auto env_to_str = [](Environment e) {return environment_to_string(e);};

        std::string environment_options = generate_options(
            environment_to_values(),
            env_to_str,
            g_configuration.environment,
            "environment"
        );

        auto db_to_str = [](DatabaseType e) {return database_type_to_string(e);};

        std::string database_type_options = generate_options(
            essential::database_type_to_values(),
            db_to_str,
            g_configuration.database_type,
            "database_type"
        );

        auto acc_to_str = [](AccessMode e) {return access_mode_to_string(e);};

        std::string access_mode_options = generate_options(
            essential::access_mode_to_values(),
            acc_to_str,
            g_configuration.access_mode,
            "access_mode"
        );

        auto reg_to_str = [](RegistrationMode e) {return registration_mode_to_string(e);};

        std::string registration_mode_options = generate_options(
            essential::registration_mode_to_values(),
            reg_to_str,
            g_configuration.registration_mode,
            "registration_mode"
        );

        auto role_to_str = [](UserRole e) {return user_role_to_string(e);};

        std::string default_user_role_options = generate_options(
            essential::user_role_to_values(),
            role_to_str,
            g_configuration.default_user_role,
            "default_user_role"
        );

        auto log_to_str = [](LogLevel e) {return log_level_to_string(e);};

        std::string max_log_level_options = generate_options(
            essential::log_level_to_values(),
            log_to_str,
            g_configuration.max_log_level,
            "max_log_level"
        );

        ////
        push_entry("name", g_configuration.name);
        push_entry("description", g_configuration.description);
        push_enum(environment)
        //
        push_entry("host", g_configuration.host);
        push_entry("port", g_configuration.port);
        push_entry("frontend_port", g_configuration.frontend_port);
        push_enum(database_type)
        //
        push_enum(access_mode)
        push_enum(registration_mode)
        push_enum(default_user_role)
        //database
        push_entry("db_host", db_host);
        push_entry("db_port", db_port);
        push_entry("db_name", db_name);
        push_entry("db_user", db_user);
        push_entry("db_password", db_password);
        //other

        push_enum(max_log_level)
        push_entry("allowed_plugins", fmt::join(g_configuration.allowed_plugins, ","));
        push_entry("access_token_expires_in", access_token_expires_in);
        push_entry("refresh_token_expires_in", refresh_token_expires_in);
        push_entry("refresh_token_rotation_threshold_in", refresh_token_rotation_threshold_in);

#undef push_enum

        return store;
    }

    std::string Configuration::validate()
    {
        if (access_token_expires_in < ACCESS_TOKEN_EXPIRES_IN_MIN_VALUE)
            return "\"Access token expires in\" must be at least " + std::to_string(ACCESS_TOKEN_EXPIRES_IN_MIN_VALUE) + " minutes";
        if (access_token_expires_in > ACCESS_TOKEN_EXPIRES_IN_MAX_VALUE)
            return "\"Access token expires in\" must be at most " + std::to_string(ACCESS_TOKEN_EXPIRES_IN_MAX_VALUE) + " minutes";

        if (refresh_token_expires_in < REFRESH_TOKEN_EXPIRES_IN_MIN_VALUE)
            return "\"Refresh token expires in\" must be at least " + std::to_string(REFRESH_TOKEN_EXPIRES_IN_MIN_VALUE) + " minutes";
        if (refresh_token_expires_in > REFRESH_TOKEN_EXPIRES_IN_MAX_VALUE)
            return "\"Refresh token expires in\" must be at most " + std::to_string(REFRESH_TOKEN_EXPIRES_IN_MAX_VALUE) + " minutes";

        int refresh_token_min = access_token_expires_in * 2;
        if (refresh_token_expires_in < refresh_token_min)
            return "\"Refresh token expires in\" must be at least twice of \"Access token expires in\"";

        if (refresh_token_rotation_threshold_in < REFRESH_TOKEN_ROTATION_THRESHOLD_MIN_VALUE)
            return "\"Refresh token rotation threshold\" must be at least " + std::to_string(REFRESH_TOKEN_ROTATION_THRESHOLD_MIN_VALUE) + " minutes";
        if (refresh_token_rotation_threshold_in > refresh_token_expires_in)
            return "\"Refresh token rotation threshold\" must not exceed \"Refresh token expires in\"";

        // rotation threshold must be between 20% and 50% of refresh token lifetime
        int min_threshold = refresh_token_expires_in * 20 / 100;
        int max_threshold = refresh_token_expires_in * 50 / 100;

        if (refresh_token_rotation_threshold_in < min_threshold || refresh_token_rotation_threshold_in > max_threshold)
        {
            return "\"Refresh token rotation threshold\" must be between 20% (" +
                   std::to_string(min_threshold) + " minutes) and 50% (" +
                   std::to_string(max_threshold) + " minutes) of \"Refresh token expires in\" (" +
                   std::to_string(refresh_token_expires_in) + " minutes)";
        }


        return "";
    }

}
