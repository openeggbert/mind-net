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


bool migrate_schema_if_needed()
{
    mindnet::Utils::trace("Migrating schema, if needed:");

    bool migrationResult = mindnet::persistence::impl::
        sqlite::SqliteDatabaseMigration::getInstance()->migrate();
    if (migrationResult)
    {
        mindnet::Utils::trace("Migrating schema: OK. Success.");
        return true;
    }
    mindnet::err << "Migrating schema: KO. Failed." << std::endl;
    return false;
}

void print_logo()
{
    std::cout << R"(
  __  __ _           _   _   _      _
 |  \/  (_)_ __   __| | | \ | | ___| |_
 | |\/| | | '_ \ / _` | |  \| |/ _ \ __|
 | |  | | | | | | (_| | | |\  |  __/ |_
 |_|  |_|_|_| |_|\__,_| |_| \_|\___|\__|

        )" << std::endl;
}

std::vector<std::string> main_args_to_vector(int argc, char** argv)
{
    std::vector<std::string> result;
    //std::cout << "Found " << argc << " arguments" << std::endl;
    //std::cout << "First argument: " << argv[0] << std::endl;
    for (int i = 1; i < argc; ++i)
    {
        //std::cout << "Found argument " << argv[i] << std::endl;
        result.push_back(argv[i]);
    }
    return result;
}

int main(int argc, char** argv)
{
    std::cout << "Starting mindnet..." << std::endl;

    print_logo();
    //std::cout << "argc: " << argc << std::endl;
    std::vector<str> arguments = main_args_to_vector(argc, argv);
    if (arguments.size() == 0)
    {
        mindnet::err << "No arguments provided. Exiting." << std::endl;
        return 1;
    }

    if (!migrate_schema_if_needed())
    {
        mindnet::err << "Failed to migrate schema. Exiting." << std::endl;
        exit(mindnet::ExitStatus::MIGRATION_FAILED);
    }

    std::shared_ptr<mindnet::persistence::Persistence> db = std::make_shared<mindnet::persistence::Persistence>();

    auto arg0 = arguments[0];
    if (arg0 == "start")
    {
        bool custom_port = false;
        int port = 8080;
        for (int i = 1; i < arguments.size(); ++i)
        {
            auto argument = arguments[i];
            if (argument[0] != '-')
            {
                mindnet::err << "Option must start with \"-\": " << argument << std::endl;
                return 1;
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
                        return 1;
                    }
                    ++i;
                }
                else
                {
                    mindnet::err << "No port provided for option --port. Exiting." << std::endl;
                    return 1;
                }
            } else
            {
                mindnet::err << "Unknown option for start command: " << argument << std::endl;
                return 1;
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
    }
    else if (arg0 == "help")
    {
        std::cout << "Help is not yet implemented." << std::endl;
    }
    else
    {
        mindnet::err << "Unknown command: " << arg0 << std::endl;
        return 1;
    }


    return 0;

}
