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
#include "mindnet/persistence/impl/sqlite/SqliteDatabaseMigration.h"
#include "mindnet/persistence/impl/sqlite/repositories/MapRepositoryImplSqlite.h"
#define add_controller(model) server.register_controller(&controller, mindnet::models::model##_DEFINITION);


void migrate_schema_if_needed()
{
    mindnet::Utils::trace("Migrating schema, if needed:");

    bool migrationResult = mindnet::persistence::impl::
        sqlite::SqliteDatabaseMigration::getInstance()->migrate();
    if (migrationResult)
    {
        mindnet::Utils::trace("Migrating schema: OK. Success.");
        return;
    }
    mindnet::err << "Migrating schema: KO. Failed." << std::endl;
    exit(mindnet::ExitStatus::MIGRATION_FAILED);
}

void print_logo()
{
    std::cout << "Starting Mind Net..." << std::endl;

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
        mindnet::err << static_cast<const char*>("No arguments provided. Exiting.") << std::endl;
        return false;
    }
    return true;
}

void load_args(int argc, char** argv, std::vector<std::string>& arguments)
{
    std::vector<std::string> result;
    //std::cout << "Found " << argc << " arguments" << std::endl;
    //std::cout << "First argument: " << argv[0] << std::endl;
    for (int i = 1; i < argc; ++i)
    {
        //std::cout << "Found argument " << argv[i] << std::endl;
        result.push_back(argv[i]);
    }
    if (!check_args(result)) exit(mindnet::ExitStatus::NO_ARGUMENT_PROVIDED);
}

bool commands_function_start(
    std::vector<std::string> arguments,
    std::shared_ptr<mindnet::persistence::Persistence> db,
    int& exit_status)
{
    bool custom_port = false;
    int port = 8080;
    for (int i = 1; i < arguments.size(); ++i)
    {
        auto argument = arguments[i];
        if (argument[0] != '-')
        {
            mindnet::err << "Option must start with \"-\": " << argument << std::endl;
            exit_status = 1;
            return true;
        }
        if (argument == "-p" || argument == "--port")
        {
            if (i + 1 < arguments.size())
            {
                try {
                    port = std::stoi(arguments[i + 1]);
                    custom_port = true;
                }
                catch (std::exception& e)
                {
                    mindnet::err << "Invalid port provided: " << arguments[i + 1] << std::endl;
                    exit_status = 1;
                    return true;
                }
                ++i;
            }
            else
            {
                mindnet::err << "No port provided for option --port. Exiting." << std::endl;
                exit_status = 1;
                return true;
            }
        } else
        {
            mindnet::err << "Unknown option for start command: " << argument << std::endl;
            exit_status = 1;
            return true;
        }
    }
    mindnet::http::HttpServer server{db};

    mindnet::routes::ModelController controller;

    add_controller(MAP)
    add_controller(CONTENT)

    if (custom_port)
    {
        std::cout << "Custom port was provided: " << port << std::endl;
    } else
    {
        std::cout << "Using default port: " << port << std::endl;
    }
    std::cout << "Starting server on port " << port << std::endl;
    mindnet::start_time = mindnet::Utils::currentUnixTimestamp();
    server.run(port);
    return false;
}

bool commands_function_help(
    std::vector<std::string> arguments,
    std::shared_ptr<mindnet::persistence::Persistence> db,
    int& exit_status)
{
    std::cout << "Help is not yet implemented." << std::endl;
    return false;
}

bool commands_function_unknown(
    std::vector<std::string> arguments,
    std::shared_ptr<mindnet::persistence::Persistence> db,
    int& exit_status)
{
    mindnet::err << "Unknown command: " << arguments[0] << std::endl;
    return false;
}

bool run_command(
    std::vector<std::string> arguments,
    std::shared_ptr<mindnet::persistence::Persistence> db)
{
    int exit_status = 0;
    auto arg0 = arguments[0];
    typedef bool (*commands_function)(
        std::vector<std::string>,
        std::shared_ptr<mindnet::persistence::Persistence> db,
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
    print_logo();
    std::vector<std::string> arguments;
    load_args(argc, argv, arguments);
    migrate_schema_if_needed();
    auto db = std::make_shared<mindnet::persistence::Persistence>();

    return run_command(arguments, db);
}
