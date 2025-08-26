//
// Created by robertvokac on 7/31/25.
//
#include <iostream>

#include "mindnet/Utils.h"
#include <memory>
#include <sstream>

#include "mindnet/Global.h"
#include "mindnet/ExitStatus.h"
#include "mindnet/http/HttpServer.h"
#include "mindnet/persistence/Persistence.h"
#include "mindnet/persistence/impl/sqlite/repositories/ContentRepositoryImplSqlite.h"
#include "mindnet/controllers/ModelController.h"
#include "mindnet/models/History.h"
#include "mindnet/models/TagType.h"
#include "mindnet/models/Map.h"
#include "mindnet/models/Content.h"
#include "mindnet/models/Node.h"
#include "mindnet/models/ExternalLink.h"
#include "mindnet/models/NodeLink.h"
#include "mindnet/models/Tag.h"
#include "mindnet/models/Property.h"
#include "mindnet/persistence/impl/sqlite/SqliteDatabaseMigration.h"
#define add_controller(model) server.register_controller(&controller, mindnet::models::model##_DEFINITION);


using mindnet::commit;

void migrate_schema_if_needed()
{
    mindnet::trace << "Migrating schema, if needed" << commit;

    bool migrationResult = mindnet::persistence::impl::
        sqlite::SqliteDatabaseMigration::getInstance()->migrate();
    if (migrationResult)
    {
        mindnet::trace << "Migrating schema: OK. Success." << commit;
        return;
    }
    mindnet::err << "Migrating schema: KO. Failed." << commit;
    exit(mindnet::ExitStatus::MIGRATION_FAILED);
}

void print_logo()
{
    mindnet::info << "Starting Mind Net..." << std::endl;

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
bool check_args(std::vector<str>& arguments)
{
    if (arguments.empty())
    {
        mindnet::fatal << static_cast<const char*>("No arguments provided. Exiting.") << commit;
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
    if (!check_args(arguments)) exit(mindnet::ExitStatus::NO_ARGUMENT_PROVIDED);
}

bool commands_function_start(
    std::vector<std::string>& arguments,
    std::shared_ptr<mindnet::persistence::Persistence>& db,
    int& exit_status)
{
    bool custom_port = false;
    int port = 8080;
    str static_directory = "static";
    for (int i = 1; i < arguments.size(); ++i)
    {
        const auto& argument = arguments[i];
        if (argument[0] != '-')
        {
            mindnet::fatal << "Option must start with \"-\": " << argument << commit;
            exit_status = 1;
            return true;
        }
        if (argument == "-p" || argument == "--port")
        {
            if (i + 1 < arguments.size())
            {
                try
                {
                    port = std::stoi(arguments[i + 1]);
                    custom_port = true;
                }
                catch (std::exception& e)
                {
                    mindnet::fatal << "Invalid port provided: " << arguments[i + 1] << commit;
                    exit_status = 1;
                    return true;
                }
                ++i;
            }
            else
            {
                mindnet::fatal << "No port provided for option --port. Exiting." << commit;
                exit_status = 1;
                return true;
            }
        }
        else if (argument == "-s" || argument == "--static-directory")
        {
            if (i + 1 < arguments.size())
            {
                static_directory = arguments[i + 1];
                ++i;
            }
            else
            {
                mindnet::fatal << "No path provided for option --static-directory. Exiting." << commit;
                exit_status = 1;
                return true;
            }
        }
        else
        {
            mindnet::fatal << "Unknown option for start command: " << argument << commit;
            exit_status = 1;
            return true;
        }
    }
    mindnet::http::HttpServer server{db, static_directory};

    mindnet::routes::ModelController controller;

    add_controller(HISTORY)
    add_controller(MAP)
    add_controller(CONTENT)
    add_controller(NODE)
    add_controller(TAG_TYPE)
    add_controller(EXTERNAL_LINK)
    add_controller(NODE_LINK)
    add_controller(NODE_TAG)
    add_controller(PROPERTY)

    if (custom_port)
    {
        mindnet::debug << "Custom port was provided: " << port << commit;
    }
    else
    {
        mindnet::debug << "Using default port: " << port << commit;
    }
    mindnet::info << "Starting server on port " << port << commit;
    mindnet::start_time = mindnet::Utils::currentUnixTimestamp();
    server.run(port);
    return false;
}

bool commands_function_help(
    std::vector<std::string>& arguments,
    std::shared_ptr<mindnet::persistence::Persistence>& db,
    int& exit_status)
{
    mindnet::warn << "Help is not yet implemented." << commit;
    return false;
}

bool commands_function_unknown(
    std::vector<std::string>& arguments,
    std::shared_ptr<mindnet::persistence::Persistence>& db,
    int& exit_status)
{
    mindnet::err << "Unknown command: " << arguments[0] << commit;
    return false;
}

bool run_command(
    std::vector<std::string>& arguments,
    std::shared_ptr<mindnet::persistence::Persistence>& db)
{
    int exit_status = 0;
    auto arg0 = arguments[0];
    typedef bool (*commands_function)(
        std::vector<std::string>&,
        std::shared_ptr<mindnet::persistence::Persistence>& db,
        int&
    );

    std::map<std::string, commands_function> commands;
    commands["start"] = commands_function_start;
    commands["help"] = commands_function_help;

    if (commands.find(arg0) == commands.end())
    {
        commands_function_unknown(arguments, db, exit_status);
        return true;
    }

    commands_function command = commands[arg0];
    command(arguments, db, exit_status);
    return exit_status;
}

int main(int argc, char** argv)
{
    auto loggers = { &mindnet::fatal, &mindnet::err, &mindnet::warn,
                 &mindnet::info, &mindnet::debug, &mindnet::trace, &mindnet::test };

    for (auto* logger : loggers) {
        logger->set_timestamp_function(&mindnet::Utils::print_current_timestamp);
    }

    print_logo();
    std::vector<std::string> arguments;
    load_args(argc, argv, arguments);
    migrate_schema_if_needed();
    auto db = std::make_shared<mindnet::persistence::Persistence>();

    return run_command(arguments, db);
}
