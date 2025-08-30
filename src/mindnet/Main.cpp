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
#include "mindnet/controllers/ModelController.h"
//
#include "mindnet/models/User.h"
#include "mindnet/models/Message.h"
#include "mindnet/models/Team.h"
#include "mindnet/models/TeamMember.h"
#include "mindnet/models/Discussion.h"
#include "mindnet/models/Comment.h"
#include "mindnet/models/Suggestion.h"
#include "mindnet/models/SuggestionReview.h"
#include "mindnet/models/History.h"
#include "mindnet/models/Map.h"
#include "mindnet/models/Content.h"
#include "mindnet/models/Note.h"
#include "mindnet/models/Property.h"
#include "mindnet/models/TagType.h"
#include "mindnet/models/Tag.h"
#include "mindnet/models/Collection.h"
#include "mindnet/models/CollectionItem.h"
#include "mindnet/models/Review.h"
#include "mindnet/models/SM2State.h"
#include "mindnet/models/Reference.h"
#include "mindnet/models/Link.h"
//
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
    std::shared_ptr<mindnet::persistence::Persistence>& db,
    int& exit_status)
{
    bool custom_port = false;
    int port = 8080;
    string static_directory = "static";
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

    add_controller(USER)
    add_controller(MESSAGE)
    add_controller(TEAM)
    add_controller(TEAM_MEMBER)
    add_controller(DISCUSSION)
    add_controller(COMMENT)
    add_controller(SUGGESTION)
    add_controller(SUGGESTION_REVIEW)
    add_controller(HISTORY)
    add_controller(MAP)
    add_controller(CONTENT)
    add_controller(NOTE)
    add_controller(PROPERTY)
    add_controller(TAG_TYPE)
    add_controller(TAG)
    add_controller(COLLECTION)
    add_controller(COLLECTION_ITEM)
    add_controller(REVIEW)
    add_controller(SM2_STATE)
    add_controller(REFERENCE)
    add_controller(LINK)
    //
    //CREATE
    CROW_ROUTE(server.get_crow_app(), "/api/model_definition").methods(crow::HTTPMethod::POST)
        ([] { return crow::response(405, "Method not allowed for model_definition.");; });

    auto column_definition_to_json = [db](mindnet::models::misc::ColumnDefinition& column_definition)
    {
        crow::json::wvalue result;

        result["column_name"] = column_definition.get_column_name();
        result["column_type"] = mindnet::enums::column_type_to_string(column_definition.get_column_type());
        result["mandatory"] = column_definition.is_mandatory();
        result["primary_key"] = column_definition.is_primary_key();
        result["foreign_key"] = column_definition.get_foreign_key();
        if (column_definition.get_enum_definition().has_value())
        {
            auto enum_ = column_definition.get_enum_definition().value();
            crow::json::wvalue::list enum_list;
            int enum_size = enum_.get_value_count();
            for (int i = 0; i < enum_size; ++i)
            {
                int number = enum_.get_value(i);
                string text = enum_.get_value_as_string(i);
                crow::json::wvalue enum_json;
                enum_json[text] = number;
                enum_list.push_back(std::move(enum_json));
            }

            result["enum_definition"] = std::move(enum_list);
        }

        result["unique"] = column_definition.is_unique();
        result["auto"] = column_definition.is_auto();
        result["default_value"] = column_definition.get_default_value();

        return result;
    };

    auto model_definition_to_json = [db, column_definition_to_json](string& model_name, std::set<string> fields_set)
    {
        auto model_definition = db->get_model_definition(model_name);
        crow::json::wvalue res;
        auto fields_set_empty = fields_set.empty();
        if (fields_set_empty || fields_set.contains("model_name"))
        {
            res["model_name"] = model_definition->get_model_name();
        }
        crow::json::wvalue::list crudl_list;
        for (auto e : model_definition->get_allowed_rest_operations())
        {
            crudl_list.push_back(mindnet::enums::crudl_to_string(e));
        }

        if (fields_set_empty || fields_set.contains("allowed_rest_operations"))
        {
            res["allowed_rest_operations"] = std::move(crudl_list);
        }
        if (fields_set_empty || fields_set.contains("virtual_table"))
        {
            res["virtual_table"] = model_definition->is_virtual_table();
        }
        //
        if (fields_set_empty || fields_set.contains("columns"))
        {
            crow::json::wvalue::list column_list;
            for (auto e : model_definition->get_columns())
            {
                auto column_as_json = column_definition_to_json(e);
                column_list.push_back(column_as_json);
            }
            res["columns"] = std::move(column_list);
        }

        return res;
    };
    //READ
    CROW_ROUTE(server.get_crow_app(), "/api/model_definition/<string>").methods(crow::HTTPMethod::GET)
    ([db, model_definition_to_json](const crow::request& req, string model_name)
    {
        if (!db->has_repository(model_name))
        {
            return crow::response(404, "Model definition not found: " + model_name);
        }
        auto json = model_definition_to_json(model_name, {});

        return crow::response(200, json);
    });

    // UPDATE
    CROW_ROUTE(server.get_crow_app(), "/api/model_definition").methods(crow::HTTPMethod::PUT)
        ([] { return crow::response(405, "Method not allowed for model_definition.");; });

    // DELETE
    CROW_ROUTE(server.get_crow_app(), "/api/model_definition").methods(crow::HTTPMethod::DELETE)
        ([] { return crow::response(405, "Method not allowed for model_definition.");; });

    //todo: remove this duplicity
    auto split_string_by_commas = [](const string& string_, std::set<std::string>& result)
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
    };

    // LIST
    CROW_ROUTE(server.get_crow_app(), "/api/model_definition").methods(crow::HTTPMethod::GET)
    ([db, model_definition_to_json, split_string_by_commas](const crow::request& req)
    {
        string fields = req.url_params.get("fields") ? req.url_params.get("fields") : "";

        std::set<string> fields_set;
        split_string_by_commas(fields, fields_set);
        crow::json::wvalue result;

        crow::json::wvalue::list model_definitions_as_json;
        for (auto& model_name : db->list_repositories())
        {
            //std::cout << model_name << std::endl;
            auto model_definition_as_json = model_definition_to_json(model_name, fields_set);
            model_definitions_as_json.push_back(model_definition_as_json);
        }
        result["items"] = std::move(model_definitions_as_json);
        return crow::response(200, result);

        //return model_definition_to_json(model_name);
    });

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
    auto db = std::make_shared<mindnet::persistence::Persistence>();

    return run_command(arguments, db);
}
