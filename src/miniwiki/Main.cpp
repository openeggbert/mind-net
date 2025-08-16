//
// Created by robertvokac on 7/31/25.
//
#include <iostream>

#include "miniwiki/Utils.h"
#include <memory>
#include <sstream>

#include "miniwiki/Global.h"
#include "miniwiki/Command/CommandFactory.h"
#include "miniwiki/Command/CommandHelper.h"
#include "miniwiki/ExitStatus.h"
#include "miniwiki/http/HttpServer.h"
#include "miniwiki/Manager/MiniWikiManager.h"
#include "miniwiki/persistence/DB.h"
#include "miniwiki/persistence/Impl/Sqlite/SqliteDatabaseMigration.h"
#include "miniwiki/persistence/Impl/Sqlite/Repositories/ContentRepositoryImplSqlite.h"
#include "miniwiki/persistence/Impl/Sqlite/Repositories/IdeaRepositoryImplSqlite.h"
#include "miniwiki/persistence/Impl/Sqlite/Repositories/NoteRepositoryImplSqlite.h"
#include "miniwiki/persistence/Impl/Sqlite/Repositories/OldContentRepositoryImplSqlite.h"
#include "miniwiki/persistence/Impl/Sqlite/Repositories/OldEntityRepositoryImplSqlite.h"
#include "miniwiki/persistence/Impl/Sqlite/Repositories/SessionRepositoryImplSqlite.h"
#include "miniwiki/persistence/Impl/Sqlite/Repositories/TermRepositoryImplSqlite.h"
#include "miniwiki/controllers/ContentController.h"

bool migrate_schema_if_needed()
{
    miniwiki::Utils::trace("Migrating schema, if needed:");

    bool migrationResult = miniwiki::Persistence::Impl::
        Sqlite::SqliteDatabaseMigration::getInstance()->migrate();
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

/**
 * Creates a session if no active session currently exists.
 *
 * This function retrieves the current session from the session manager
 * within the provided MiniWikiManager instance. If the session's ID is 0
 * (indicating no existing session), a new session is created with a default
 * path of "/", the current Unix timestamp as the last opened time,
 * and an ID value of 1. This new session is then saved using the session
 * manager's `create` function.
 *
 * @param note_box_manager An instance of miniwiki::Manager::MiniWikiManager,
 *                         which contains the session manager and is used
 *                         to manage session operations.
 * @return `true` if a new session was created; `false` if a session
 *         already exists.
 */
bool create_session_if_does_not_yet_exist(miniwiki::Manager::MiniWikiManager note_box_manager)
{
    auto session = note_box_manager.session_manager.get();
    if (session.id == 0)
    {
        session.id = 1;
        session.current_path = "";
        session.last_opened = miniwiki::Utils::currentUnixTimestamp();
        note_box_manager.session_manager.create(session);
        return true;
    }
    else { return false; }
}

bool set_editor_if_needed(const std::shared_ptr<miniwiki::Persistence::DB>& db, int& exit_status)
{
    while (db->session_repository->get().editor_path.empty())
    {
        miniwiki::err << "Please set the editor path. It must be a valid program." << std::endl;
        std::cout << "Editor path: ";
        std::string editor_path;
        std::getline(std::cin, editor_path);
        db->session_repository->get().editor_path = editor_path;
        if (editor_path.empty())
        {
            miniwiki::err << "Editor path cannot be empty." << std::endl;
            if (!miniwiki::Utils::ask_yes_no("Do you want to type the editor path?"))
            {
                std::cout << "Exiting application" << std::endl;
                exit_status = miniwiki::ExitStatus::EDITOR_PATH_NOT_SET;
                return true;
            }
        }
        else
        {
            auto current_session = db->session_repository->get();
            current_session.editor_path = editor_path;
            db->session_repository->update(current_session);
            break;
        }
        std::cin.clear();
    }
    return false;
}

std::vector<std::string> main_args_to_vector(int argc, char** argv)
{
    std::vector<std::string> result;
    for (int i = 1; i < argc; ++i)
    {
        result.push_back(argv[i]);
    }
    return result;
}

int main(int argc, char** argv)
{
    std::vector<str> arguments = main_args_to_vector(argc, argv);
    if (arguments.size() == 0)
    {
        miniwiki::err << "No arguments provided. Exiting." << std::endl;
        return 1;
    }
    auto arg0 = arguments[0];
    if (arg0 == "start")
    {
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
        miniwiki::http::HttpServer server;

        miniwiki::routes::ContentController content_controller;
        server.register_controller(&content_controller);
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
    miniwiki::start_time = miniwiki::Utils::currentUnixTimestamp();
    print_logo();

    if (!migrate_schema_if_needed())
    {
        miniwiki::err << "Failed to migrate schema. Exiting." << std::endl;
        exit(miniwiki::ExitStatus::MIGRATION_FAILED);
    }

    std::shared_ptr<miniwiki::Persistence::DB> db = std::make_shared<miniwiki::Persistence::DB>();

    miniwiki::Impl::Sqlite::Repositories::LiteratureSourceRepositoryImplSqlite literature_source_repository{};
    miniwiki::Impl::Sqlite::Repositories::SessionRepositoryImplSqlite session_repository{};
    miniwiki::Impl::Sqlite::Repositories::ContentRepositoryImplSqlite content_repository{};
    miniwiki::Impl::Sqlite::Repositories::OldContentRepositoryImplSqlite old_content_repository{};
    miniwiki::Impl::Sqlite::Repositories::OldEntityRepositoryImplSqlite old_entity_repository{};
    miniwiki::Impl::Sqlite::Repositories::NoteRepositoryImplSqlite note_repository{};
    miniwiki::Impl::Sqlite::Repositories::TermRepositoryImplSqlite term_repository{};
    miniwiki::Impl::Sqlite::Repositories::IdeaRepositoryImplSqlite idea_repository{};

    db->literature_source_repository = &literature_source_repository;
    db->session_repository = &session_repository;
    db->note_repository = &note_repository;
    db->content_repository = &content_repository;
    db->old_content_repository = &old_content_repository;
    db->old_entity_repository = &old_entity_repository;
    db->term_repository = &term_repository;
    db->idea_repository = &idea_repository;

    miniwiki::Manager::MiniWikiManager note_box_manager(db);

    create_session_if_does_not_yet_exist(note_box_manager);

    int exit_status;
    if (set_editor_if_needed(db, exit_status)) return exit_status;
    miniwiki::Command::CommandFactory factory;
    miniwiki::Command::CommandHelper command_helper(&factory, &db);
    factory.getCommand("help")->setCommandHelper(&command_helper);
    factory.getCommand("walk")->setCommandHelper(&command_helper);
    factory.getCommand("tree")->setCommandHelper(&command_helper);
    factory.getCommand("term")->setCommandHelper(&command_helper);
    factory.getCommand("cd")->execute(note_box_manager, note_box_manager.session_manager.get().current_path);

    std::string line;

    while (true)
    {
        std::cout << "> " << std::flush;

        if (!std::getline(std::cin, line)) break;

        std::istringstream iss(line);
        std::string cmd, args;
        iss >> cmd;
        std::getline(iss, args);
        if (!args.empty() && args[0] == ' ') args = args.substr(1);

        if (cmd == "exit" || cmd == "quit")
        {
            std::cout << "Exiting application" << std::endl;
            break;
        }

        auto command = factory.getCommand(cmd);
        if (command)
        {
            command->execute(note_box_manager, args);
        }
        else if (!cmd.empty())
        {
            miniwiki::err << cmd << " is not a miniwiki command. See 'help'." << std::endl;
        }

        // Show path after the command
        // std::cout << ":" << note_box_manager.note_manager.pwd() << "\n";
    }

    auto session = note_box_manager.session_manager.get();
    session.last_opened = miniwiki::Utils::currentUnixTimestamp();
    session.current_path = note_box_manager.note_manager.pwd();
    note_box_manager.session_manager.update(session);

    return 0;
}
