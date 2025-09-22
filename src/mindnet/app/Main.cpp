//
// Created by robertvokac on 7/31/25.
//
//

//
#include <iostream>
#include <filesystem>

#include "mindnet/other/util/Utils.h"
#include <memory>

#include "mindnet/core/Configuration.h"
#include "mindnet/core/Global.h"
#include "mindnet/core/ExitStatus.h"
#include "mindnet/other/http/HttpServer.h"
#include "mindnet/other/api/Persistence.h"
#include "mindnet/other/http/ModelEndpointGenerator.h"

#include "mindnet/other/api/IService.h"
#include "mindnet/core/Service.h"
#include "mindnet/other/api/PluginRegistry.h"
#include "mindnet/other/db/sqlite/SqliteDatabaseMigration.h"
#include "mindnet/other/plugins/chat/ChatPluginFactory.h"
#include "mindnet/other/plugins/core/CorePluginFactory.h"
#include "mindnet/other/plugins/mail/MailPluginFactory.h"
#include "mindnet/other/plugins/suggestion/SuggestionPluginFactory.h"
#include "mindnet/other/plugins/supermemo/SuperMemoPluginFactory.h"
#include "mindnet/other/plugins/slipbox/SlipBoxPluginFactory.h"

#define REGISTER_PLUGIN(plugin, Plugin) plugin_registry->register_plugin(mindnet::plugins:: plugin :: Plugin##PluginFactory().create());
using mindnet::core::commit;
using mindnet::core::g_configuration;
using mindnet::core::ExitStatus;
using_loggers()

void migrate_schema_if_needed(mindnet::api::PluginRegistryPtr& plugin_registry_ptr)
{
    trace << "Migrating schema, if needed" << commit;

    mindnet::core::DatabaseType database_type = g_configuration.database_type;
    if (database_type != mindnet::core::DatabaseType::SQLite)
    {
        err << "SQLite database is only supported, but you configured " <<
            mindnet::core::database_type_to_string(database_type) << commit;
        exit(ExitStatus::MIGRATION_FAILED);
    }
    for (auto& plugin_name : plugin_registry_ptr->get_plugin_names_sorted_by_dependencies())
    {
        debug << "Migrating schema for plugin " << plugin_name << commit;
        auto plugin = plugin_registry_ptr->get_plugin(plugin_name);
        auto migration_scripts = plugin->get_migration_scripts();
        if (migration_scripts == nullptr) {continue;}
        bool migration_result =
            mindnet::db::sqlite::SqliteDatabaseMigration::
            getInstance()->migrate(plugin_name, migration_scripts);
        if (migration_result)
        {
            trace << "Migrating schema for plugin " << plugin_name << ": OK. Success." << commit;
            migration_scripts.reset();
            continue;
        }
        migration_scripts.reset();
        err << "Migrating schema for plugin " << plugin_name << ": KO. Failed." << commit;
        exit(ExitStatus::MIGRATION_FAILED);
    }

}

void print_logo()
{
    info << "Starting Mind Net..." << std::endl;

    std::cout << R"(
  __  __ _           _   _   _      _
 |  \/  (_)_ __   __| | | \ | | ___| |_
 | |\/| | | '_ \ / _` | |  \| |/ _ \ __|
 | |  | | | | | | (_| | | |\  |  __/ |_
 |_|  |_|_|_| |_|\__,_| |_| \_|\___|\__|

        )" << std::endl;
}

/**
 * Validates the provided arguments based on predefined constraints or conditions.
 *
 * @param args A vector of strings representing the arguments to validate.
 * @return A boolean value indicating whether all arguments meet the validation criteria (true if valid, false otherwise).
 */
bool check_args(std::vector<string>& arguments)
{
    if (arguments.empty())
    {
        fatal << static_cast<const char*>("No arguments provided. Exiting.") << commit;
        return false;
    }
    return true;
}

void load_args(int argc, char** argv, std::vector<std::string>& arguments)
{
    for (int i = 1; i < argc; ++i)
    {
        arguments.push_back(argv[i]);
    }
    if (!check_args(arguments)) exit(ExitStatus::NO_ARGUMENT_PROVIDED);
}

bool commands_function_start(
    std::vector<std::string>& arguments,
    std::shared_ptr<mindnet::api::IService>& service_ptr,
    int& exit_status)
{
    bool custom_host = false;
    bool custom_port = false;
    bool custom_frontend_port = false;

    int port = 8080;
    int frontend_port = port;
    string host = "http://localhost";
    string static_directory = "static";

    auto require_value = [&](int& i, const std::string& option) -> std::string
    {
        if (i + 1 >= arguments.size())
        {
            fatal << "No value provided for option " << option << ". Exiting." << commit;
            exit_status = 1;
            throw std::runtime_error(std::string("Missing argument for ") + option);
        }
        return arguments[++i]; // consume next argument
    };

    auto parse_port = [&](const std::string& value, const std::string& what) -> int
    {
        try
        {
            int p = std::stoi(value);
            if (p < 1 || p > 65535)
            {
                fatal << what << " must be between 1 and 65535" << commit;
                exit_status = 1;
                throw std::runtime_error("invalid port range");
            }
            return p;
        }
        catch (...)
        {
            fatal << "Invalid " << what << " provided: " << value << commit;
            exit_status = 1;
            throw;
        }
    };

    for (int i = 1; i < arguments.size(); ++i)
    {
        const auto& argument = arguments[i];
        if (argument[0] != '-')
        {
            fatal << "Option must start with \"-\": " << argument << commit;
            exit_status = 1;
            return true;
        }

        if (argument == "-p" || argument == "--port")
        {
            port = parse_port(require_value(i, argument), "Port");
            custom_port = true;
        }
        else if (argument == "-f" || argument == "--frontend-port")
        {
            frontend_port = parse_port(require_value(i, argument), "Frontend port");
            custom_frontend_port = true;
        }
        else if (argument == "-h" || argument == "--host")
        {
            host = require_value(i, argument);
            custom_host = true;
        }
        else if (argument == "-s" || argument == "--static-directory")
        {
            static_directory = require_value(i, argument);
        }
        else
        {
            fatal << "Unknown option for start command: " << argument << commit;
            exit_status = 1;
            return true;
        }
    }

    if (!std::filesystem::exists(static_directory) || !std::filesystem::is_directory(static_directory))
    {
        fatal << "Static directory does not exist: " << static_directory << commit;
        exit_status = 1;
        return true;
    }

    mindnet::http::HttpServer server{service_ptr, static_directory};

    mindnet::http::ModelEndpointGenerator controller;

    for (auto& plugin_name : service_ptr->get_plugin_registry()->get_plugin_names_sorted_by_dependencies())
    {
        auto plugin = service_ptr->get_plugin_registry()->get_plugin(plugin_name);
        for (auto& model_registration : plugin->get_model_registrations())
        {
            server.create_model_endpoint(&controller, model_registration->model_definition);
        }
    }

    if (custom_port) { debug << "Custom port was provided: " << port << commit; }
    else { debug << "Using default port: " << port << commit; }

    if (custom_frontend_port) { debug << "Custom frontend port was provided: " << frontend_port << commit; }
    else { debug << "Using default frontend port: " << frontend_port << commit; }

    if (custom_host) { debug << "Custom host was provided: " << host << commit; }
    else { debug << "Using default host: " << host << commit; }

    info << "Starting backend on port " << port << commit;
    info << "Starting frontend on port " << frontend_port << commit;
    mindnet::core::start_time = mindnet::util::Utils::currentUnixTimestamp();
    g_configuration.host = host;
    g_configuration.port = port;
    g_configuration.frontend_port = frontend_port;
    server.run(host, port, frontend_port);
    return false;
}

bool commands_function_help(
    std::vector<std::string>& arguments,
    std::shared_ptr<mindnet::api::IService>& service_ptr,
    int& exit_status)
{
    warn << "Help is not yet implemented." << commit;
    return false;
}

bool commands_function_unknown(
    std::vector<std::string>& arguments,
    std::shared_ptr<mindnet::api::IService>& service_ptr,
    int& exit_status)
{
    err << "Unknown command: " << arguments[0] << commit;
    return false;
}

bool run_command(
    std::vector<std::string>& arguments,
    std::shared_ptr<mindnet::api::IService>& service_ptr)
{
    int exit_status = 0;
    auto arg0 = arguments[0];
    typedef bool (*commands_function)(
        std::vector<std::string>&,
        std::shared_ptr<mindnet::api::IService>& service_ptr,
        int&
    );

    std::map<std::string, commands_function> commands;
    commands["start"] = commands_function_start;
    commands["help"] = commands_function_help;

    if (commands.find(arg0) == commands.end())
    {
        commands_function_unknown(arguments, service_ptr, exit_status);
        return true;
    }

    commands_function command = commands[arg0];
    command(arguments, service_ptr, exit_status);
    return exit_status;
}

void register_plugins(const std::shared_ptr<mindnet::api::PluginRegistry>& plugin_registry)
{
    REGISTER_PLUGIN(core, Core)
    REGISTER_PLUGIN(slipbox, SlipBox)
    // REGISTER_PLUGIN(supermemo, SuperMemo)
    // REGISTER_PLUGIN(mail, Mail)
    // REGISTER_PLUGIN(chat, Chat)
    // REGISTER_PLUGIN(suggestion, Suggestion)
}

int main(int argc, char** argv)
{
    mindnet::core::start_time = mindnet::util::Utils::currentUnixTimestamp();

    auto loggers = {
        &fatal, &err, &warn, &info, &debug, &trace, &experiment
    };

    for (auto* logger : loggers)
    {
        logger->set_timestamp_function(&mindnet::util::Utils::print_current_timestamp);
    }

    print_logo();
    std::vector<std::string> arguments;
    load_args(argc, argv, arguments);

    mindnet::api::PluginRegistryPtr plugin_registry_ptr = std::make_shared<mindnet::api::PluginRegistry>();
    register_plugins(plugin_registry_ptr);

    migrate_schema_if_needed(plugin_registry_ptr);

    std::shared_ptr<mindnet::api::IPersistence> db = std::make_shared<
        mindnet::api::Persistence>(plugin_registry_ptr);

    std::shared_ptr<mindnet::api::IService> service = std::make_shared<mindnet::core::Service>(db, plugin_registry_ptr);
    return run_command(arguments, service);
}
