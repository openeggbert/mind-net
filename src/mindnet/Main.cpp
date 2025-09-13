//
// Created by robertvokac on 7/31/25.
//
//
#define plugin_base
#define plugin_zettelkasten
#define plugin_email
#define plugin_chat
#define plugin_suggestion
#define plugin_test
//
#include <iostream>
#include <filesystem>

#include "mindnet/Utils.h"
#include <memory>
#include <sstream>

#include "mindnet/Global.h"
#include "mindnet/ExitStatus.h"
#include "mindnet/http/HttpServer.h"
#include "mindnet/persistence/api/Persistence.h"
#include "mindnet/controllers/ModelController.h"
//
#ifdef plugin_base
#include "mindnet/models/User.h"
#include "mindnet/models/Team.h"
#include "mindnet/models/TeamMember.h"
#include "mindnet/models/History.h"
#endif


#ifdef plugin_zettelkasten
#include "mindnet/models/Map.h"
#include "mindnet/models/Content.h"
#include "mindnet/models/Note.h"
#include "mindnet/models/Property.h"
#include "mindnet/models/TagType.h"
#include "mindnet/models/Tag.h"
#include "mindnet/models/Collection.h"
#include "mindnet/models/CollectionItem.h"
#include "mindnet/models/Reference.h"
#include "mindnet/models/Link.h"
#endif

#ifdef plugin_email
#include "mindnet/models/Message.h"
#endif

#ifdef plugin_chat
#include "mindnet/models/Discussion.h"
#include "mindnet/models/Comment.h"
#endif

#ifdef plugin_suggestion
#include "mindnet/models/Suggestion.h"
#include "mindnet/models/SuggestionReview.h"
#endif

#ifdef plugin_test
#include "mindnet/models/Review.h"
#include "mindnet/models/SM2State.h"
#include "mindnet/models/Question.h"
#endif

//
#include "mindnet/IService.h"
#include "mindnet/Service.h"
#include "mindnet/persistence/impl/sqlite/SqliteDatabaseMigration.h"
#include "mindnet/persistence/impl/sqlite/validators/CollectionCrudlValidator.h"
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
bool check_args(std::vector<string>& arguments)
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
    std::shared_ptr<mindnet::IService>& service_ptr,
    int& exit_status)
{
    bool custom_host = false;
    bool custom_port = false;
    bool custom_frontend_port = false;

    int port = 8080;
    int frontend_port = port;
    string host = "http://localhost";
    string static_directory = "static";

    const char* env_secret = std::getenv("JWT_SECRET");
    if (!env_secret)
    {
        mindnet::err << "JWT_SECRET environment variable is not set." << commit;
        return 1;
    }

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
                    if (port < 1 || port > 65535)
                    {
                        mindnet::fatal << "Port must be between 1 and 65535" << commit;
                        exit_status = 1;
                        return true;
                    }
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
        else if (argument == "-f" || argument == "--frontend-port")
        {
            if (i + 1 < arguments.size())
            {
                try
                {
                    frontend_port = std::stoi(arguments[i + 1]);
                    if (frontend_port < 1 || frontend_port > 65535)
                    {
                        mindnet::fatal << "Frontend port must be between 1 and 65535" << commit;
                        exit_status = 1;
                        return true;
                    }
                    custom_frontend_port = true;
                }
                catch (std::exception& e)
                {
                    mindnet::fatal << "Invalid frontend port provided: " << arguments[i + 1] << commit;
                    exit_status = 1;
                    return true;
                }
                ++i;
            }
            else
            {
                mindnet::fatal << "No port provided for option --frontend-port. Exiting." << commit;
                exit_status = 1;
                return true;
            }
        }
        else if (argument == "-h" || argument == "--host")
        {
            if (i + 1 < arguments.size())
            {
                host = arguments[i + 1];
                custom_host = true;
                ++i;
            }
            else
            {
                mindnet::fatal << "No host provided for option --host. Exiting." << commit;
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
    if (!std::filesystem::exists(static_directory) || !std::filesystem::is_directory(static_directory))
    {
        mindnet::fatal << "Static directory does not exist: " << static_directory << commit;
        exit_status = 1;
        return true;
    }
    

    mindnet::http::HttpServer server{service_ptr, static_directory};

    mindnet::routes::ModelController controller;


#ifdef plugin_base
    add_controller(USER)
    add_controller(TEAM)
    add_controller(TEAM_MEMBER)
    add_controller(HISTORY)
#endif

#ifdef plugin_zettelkasten
    add_controller(MAP)
    add_controller(CONTENT)
    add_controller(NOTE)
    add_controller(PROPERTY)
    add_controller(TAG_TYPE)
    add_controller(TAG)
    add_controller(COLLECTION)
    add_controller(COLLECTION_ITEM)
    add_controller(REFERENCE)
    add_controller(LINK)
#endif

#ifdef plugin_email
    add_controller(MESSAGE)
#endif

#ifdef plugin_chat
    add_controller(DISCUSSION)
    add_controller(COMMENT)
#endif

#ifdef plugin_suggestion
    add_controller(SUGGESTION)
    add_controller(SUGGESTION_REVIEW)
#endif

#ifdef plugin_test
    add_controller(REVIEW)
    add_controller(SM2_STATE)
    add_controller(QUESTION)
#endif










    //


    if (custom_port) {mindnet::debug << "Custom port was provided: " << port << commit;}
    else {mindnet::debug << "Using default port: " << port << commit;}

    if (custom_frontend_port) {mindnet::debug << "Custom frontend port was provided: " << frontend_port << commit;}
    else {mindnet::debug << "Using default frontend port: " << frontend_port << commit;}

    if (custom_host) {mindnet::debug << "Custom host was provided: " << host << commit;}
    else {mindnet::debug << "Using default host: " << host << commit;}

    mindnet::info << "Starting backend on port " << port << commit;
    mindnet::info << "Starting frontend on port " << frontend_port << commit;
    mindnet::start_time = mindnet::Utils::currentUnixTimestamp();
    server.run(host, port, frontend_port);
    return false;
}

bool commands_function_help(
    std::vector<std::string>& arguments,
    std::shared_ptr<mindnet::IService>& service_ptr,
    int& exit_status)
{
    mindnet::warn << "Help is not yet implemented." << commit;
    return false;
}

bool commands_function_unknown(
    std::vector<std::string>& arguments,
    std::shared_ptr<mindnet::IService>& service_ptr,
    int& exit_status)
{
    mindnet::err << "Unknown command: " << arguments[0] << commit;
    return false;
}

bool run_command(
    std::vector<std::string>& arguments,
    std::shared_ptr<mindnet::IService>& service_ptr)
{
    int exit_status = 0;
    auto arg0 = arguments[0];
    typedef bool (*commands_function)(
        std::vector<std::string>&,
        std::shared_ptr<mindnet::IService>& service_ptr,
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

int main(int argc, char** argv)
{
    auto loggers = {
        &mindnet::fatal, &mindnet::err, &mindnet::warn,
        &mindnet::info, &mindnet::debug, &mindnet::trace, &mindnet::test
    };

    for (auto* logger : loggers)
    {
        logger->set_timestamp_function(&mindnet::Utils::print_current_timestamp);
    }

    print_logo();
    std::vector<std::string> arguments;
    load_args(argc, argv, arguments);
    migrate_schema_if_needed();
    std::shared_ptr<mindnet::persistence::api::IPersistence> db = std::make_shared<
        mindnet::persistence::api::Persistence>();
    std::shared_ptr<mindnet::IService> service = std::make_shared<mindnet::Service>(db);
    return run_command(arguments, service);
}
