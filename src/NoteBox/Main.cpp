//
// Created by robertvokac on 7/31/25.
//
#include <iostream>

#include "NoteBox/Utils.h"
#include <memory>
#include <sstream>

#include "NoteBox/Global.h"
#include "NoteBox/Command/CommandFactory.h"
#include "NoteBox/Command/HelpPrinter.h"
#include "../../include/NoteBox/ExitStatus.h"
#include "NoteBox/Manager/NoteBoxManager.h"
#include "NoteBox/Persistence/DB.h"
#include "NoteBox/Persistence/Impl/Sqlite/SqliteDatabaseMigration.h"
#include "NoteBox/Persistence/Impl/Sqlite/Repositories/ContentRepositoryImplSqlite.h"
#include "NoteBox/Persistence/Impl/Sqlite/Repositories/NoteRepositoryImplSqlite.h"
#include "NoteBox/Persistence/Impl/Sqlite/Repositories/SessionRepositoryImplSqlite.h"

bool migrateSchemaIfNeeded()
{
    NoteBox::Utils::trace("Migrating schema, if needed:");

    bool migrationResult = NoteBox::Persistence::Impl::
        Sqlite::SqliteDatabaseMigration::getInstance()->migrate();
    if (migrationResult)
    {
        NoteBox::Utils::trace("Migrating schema: OK. Success.");
        return true;
    }
    NoteBox::err << "Migrating schema: KO. Failed." << std::endl;
    return false;
}

void print_logo()
{
    std::cout << R"(
 _   _       _         ____
| \ | | ___ | |_ ___  | __ )  _____  __
|  \| |/ _ \| __/ _ \ |  _ \ / _ \ \/ /
| |\  | (_) | ||  __/ | |_) | (_) >  <
|_| \_|\___/ \__\___| |____/ \___/_/\_\
        )" << std::endl;
}

/**
 * Creates a session if no active session currently exists.
 *
 * This function retrieves the current session from the session manager
 * within the provided NoteBoxManager instance. If the session's ID is 0
 * (indicating no existing session), a new session is created with a default
 * path of "/", the current Unix timestamp as the last opened time,
 * and an ID value of 1. This new session is then saved using the session
 * manager's `create` function.
 *
 * @param note_box_manager An instance of NoteBox::Manager::NoteBoxManager,
 *                         which contains the session manager and is used
 *                         to manage session operations.
 * @return `true` if a new session was created; `false` if a session
 *         already exists.
 */
bool create_session_if_does_not_yet_exist(NoteBox::Manager::NoteBoxManager note_box_manager)
{
    auto session = note_box_manager.session_manager.get();
    if (session.id == 0)
    {
        session.id = 1;
        session.current_path = "/";
        session.last_opened = NoteBox::Utils::currentUnixTimestamp();
        note_box_manager.session_manager.create(session);
        return true;
    }
    else { return false; }
}



int main()
{
    NoteBox::start_time = NoteBox::Utils::currentUnixTimestamp();
    print_logo();

    if (!migrateSchemaIfNeeded())
    {
        NoteBox::err << "Failed to migrate schema. Exiting." << std::endl;
        exit(NoteBox::ExitStatus::MIGRATION_FAILED);
    }

    std::shared_ptr<NoteBox::Persistence::DB> db = std::make_shared<NoteBox::Persistence::DB>();

    NoteBox::Impl::Sqlite::Repositories::LiteratureSourceRepositoryImplSqlite literature_source_repository{};
    NoteBox::Impl::Sqlite::Repositories::SessionRepositoryImplSqlite session_repository{};
    NoteBox::Impl::Sqlite::Repositories::ContentRepositoryImplSqlite content_repository{};
    NoteBox::Impl::Sqlite::Repositories::NoteRepositoryImplSqlite note_repository{};

    db->literature_source_repository = &literature_source_repository;
    db->session_repository = &session_repository;
    db->note_repository = &note_repository;
    db->content_repository = &content_repository;

    NoteBox::Manager::NoteBoxManager note_box_manager(db);

    create_session_if_does_not_yet_exist(note_box_manager);

    NoteBox::Command::CommandFactory factory;
    NoteBox::Command::HelpPrinter help_printer(&factory);
    factory.getCommand("help")->setHelpPrinter(&help_printer);

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
            NoteBox::err << "Unknown command: " << cmd << std::endl;
        }

        // Show path after the command
        // std::cout << ":" << note_box_manager.note_manager.pwd() << "\n";
    }

    auto session = note_box_manager.session_manager.get();
    session.last_opened = NoteBox::Utils::currentUnixTimestamp();
    note_box_manager.session_manager.update(session);

    return 0;
}

