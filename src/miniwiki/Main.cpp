//
// Created by robertvokac on 7/31/25.
//
#include <iostream>

#include "miniwiki/Utils.h"
#include <memory>
#include <sstream>

#include "miniwiki/Global.h"
#include "miniwiki/ExitStatus.h"
#include "miniwiki/http/HttpServer.h"
#include "miniwiki/persistence/Persistence.h"
#include "miniwiki/persistence/impl/sqlite/repositories/ContentRepositoryImplSqlite.h"
#include "miniwiki/controllers/ContentController.h"
#include "miniwiki/persistence/impl/sqlite/SqliteDatabaseMigration.h"

bool migrate_schema_if_needed()
{
    miniwiki::Utils::trace("Migrating schema, if needed:");

    bool migrationResult = miniwiki::persistence::impl::
        sqlite::SqliteDatabaseMigration::getInstance()->migrate();
    if (migrationResult)
    {
        miniwiki::Utils::trace("Migrating schema: OK. Success.");
        return true;
    }
    miniwiki::err << "Migrating schema: KO. Failed." << std::endl;
    return false;
}

void print_logo()
{
    std::cout << R"(
  __  __ _       _  __        ___ _    _
 |  \/  (_)_ __ (_) \ \      / (_) | _(_)
 | |\/| | | '_ \| |  \ \ /\ / /| | |/ / |
 | |  | | | | | | |   \ V  V / | |   <| |
 |_|  |_|_|_| |_|_|    \_/\_/  |_|_|\_\_|

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
    std::cout << "Starting miniwiki..." << std::endl;

    print_logo();
    //std::cout << "argc: " << argc << std::endl;
    std::vector<str> arguments = main_args_to_vector(argc, argv);
    if (arguments.size() == 0)
    {
        miniwiki::err << "No arguments provided. Exiting." << std::endl;
        return 1;
    }

    if (!migrate_schema_if_needed())
    {
        miniwiki::err << "Failed to migrate schema. Exiting." << std::endl;
        exit(miniwiki::ExitStatus::MIGRATION_FAILED);
    }

    std::shared_ptr<miniwiki::persistence::Persistence> db = std::make_shared<miniwiki::persistence::Persistence>();
    miniwiki::impl::sqlite::repositories::ContentRepositoryImplSqlite content_repository{};
    db->content_repository = &content_repository;

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
                miniwiki::err << "Option must start with \"-\": " << argument << std::endl;
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
                        miniwiki::err << "Invalid port provided: " << arguments[i + 1] << std::endl;
                        return 1;
                    }
                    ++i;
                }
                else
                {
                    miniwiki::err << "No port provided for option --port. Exiting." << std::endl;
                    return 1;
                }
            } else
            {
                miniwiki::err << "Unknown option for start command: " << argument << std::endl;
                return 1;
            }
        }
        miniwiki::http::HttpServer server{db};

        miniwiki::routes::ContentController content_controller;
        server.register_controller(&content_controller, miniwiki::models::CONTENT_DEFINITION);
        if (custom_port)
        {
            std::cout << "Custom port was provided: " << port << std::endl;
        } else
        {
            std::cout << "Using default port: " << port << std::endl;
        }
        std::cout << "Starting server on port " << port << std::endl;
        miniwiki::start_time = miniwiki::Utils::currentUnixTimestamp();
        server.run(port);
    }
    else if (arg0 == "help")
    {
        std::cout << "Help is not yet implemented." << std::endl;
    }
    else
    {
        miniwiki::err << "Unknown command: " << arg0 << std::endl;
        return 1;
    }


    return 0;

}
