//
// Created by robertvokac on 7/31/25.
//
#include <iostream>

#include "NoteBox/Utils.h"
#include <memory>
#include <optional>
#include <sstream>

#include "NoteBox/Global.h"
#include "NoteBox/Command/CommandFactory.h"
#include "NoteBox/Command/CommandHelper.h"
#include "../../include/NoteBox/ExitStatus.h"
#include "NoteBox/Manager/NoteBoxManager.h"
#include "NoteBox/Persistence/DB.h"
#include "NoteBox/Persistence/Impl/Sqlite/SqliteDatabaseMigration.h"
#include "NoteBox/Persistence/Impl/Sqlite/Repositories/ContentRepositoryImplSqlite.h"
#include "NoteBox/Persistence/Impl/Sqlite/Repositories/NoteRepositoryImplSqlite.h"
#include "NoteBox/Persistence/Impl/Sqlite/Repositories/OldContentRepositoryImplSqlite.h"
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
        session.current_path = "";
        session.last_opened = NoteBox::Utils::currentUnixTimestamp();
        note_box_manager.session_manager.create(session);
        return true;
    }
    else { return false; }
}


/**
 * Calculates the Levenshtein distance between two strings.
 *
 * The Levenshtein distance is a measure of the similarity between two strings,
 * defined as the minimum number of single-character edits required to transform
 * one string into the other. The allowed operations are insertion, deletion, and
 * substitution of characters.
 *
 * This function uses dynamic programming to efficiently compute the distance in
 * O(m * n) time, where `m` and `n` are the lengths of the two input strings.
 *
 * @param a The first string to compare.
 * @param b The second string to compare.
 * @return The calculated Levenshtein distance between the two strings.
 */
int levenshtein(const std::string& a, const std::string& b)
{
    int m = a.size();
    int n = b.size();
    std::vector<std::vector<int>> dp(m + 1, std::vector<int>(n + 1));

    for (int i = 0; i <= m; ++i) dp[i][0] = i;
    for (int j = 0; j <= n; ++j) dp[0][j] = j;

    for (int i = 1; i <= m; ++i)
        for (int j = 1; j <= n; ++j)
            dp[i][j] = std::min(std::min(
                                    dp[i - 1][j] + 1, // deletion
                                    dp[i][j - 1] + 1), // insertion
                                dp[i - 1][j - 1] + (a[i - 1] != b[j - 1]) // substitution
            );

    return dp[m][n];
}

/**
 * Finds the closest matching command to the given input within a specified
 * maximum Levenshtein distance.
 *
 * This function iterates through a list of commands and calculates the
 * Levenshtein distance between the input string and each command string. It
 * identifies the command with the smallest distance, provided it is less than
 * or equal to the given maximum distance. If no command meets the criteria,
 * the function returns an empty optional.
 *
 * @param input The user-provided input string to match against the command list.
 * @param commands A vector containing the list of available commands.
 * @param max_distance The maximum allowable Levenshtein distance for a match
 *                     to be considered valid. Defaults to 3.
 * @return An optional containing the closest matching command if a valid
 *         match is found, or an empty optional if no such match exists.
 */
std::optional<std::string> findClosestCommand(const std::string& input, const std::vector<std::string>& commands,
                                              int max_distance = 3)
{
    int minDistance = INT_MAX;
    std::string closest;

    for (const auto& cmd : commands)
    {
        int dist = levenshtein(input, cmd);
        if (dist < minDistance)
        {
            minDistance = dist;
            closest = cmd;
        }
    }

    return minDistance <= max_distance ? std::make_optional(closest) : std::nullopt;
}


bool set_editor_if_needed(const std::shared_ptr<NoteBox::Persistence::DB>& db, int& exit_status)
{
    while (db->session_repository->get().editor_path.empty())
    {
        NoteBox::err << "Please set the editor path. It must be a valid program." << std::endl;
        std::cout << "Editor path: ";
        std::string editor_path;
        std::getline(std::cin, editor_path);
        db->session_repository->get().editor_path = editor_path;
        if (editor_path.empty())
        {
            NoteBox::err << "Editor path cannot be empty." << std::endl;
            if (!NoteBox::Utils::ask_yes_no("Do you want to type the editor path?"))
            {
                std::cout << "Exiting application" << std::endl;
                exit_status = NoteBox::ExitStatus::EDITOR_PATH_NOT_SET;
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
    NoteBox::Impl::Sqlite::Repositories::OldContentRepositoryImplSqlite old_content_repository{};
    NoteBox::Impl::Sqlite::Repositories::NoteRepositoryImplSqlite note_repository{};

    db->literature_source_repository = &literature_source_repository;
    db->session_repository = &session_repository;
    db->note_repository = &note_repository;
    db->content_repository = &content_repository;
    db->old_content_repository = &old_content_repository;

    NoteBox::Manager::NoteBoxManager note_box_manager(db);

    create_session_if_does_not_yet_exist(note_box_manager);

    int exit_status;
    if (set_editor_if_needed(db, exit_status)) return exit_status;
    NoteBox::Command::CommandFactory factory;
    NoteBox::Command::CommandHelper command_helper(&factory, &db);
    factory.getCommand("help")->setCommandHelper(&command_helper);
    factory.getCommand("walk")->setCommandHelper(&command_helper);
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
            NoteBox::err << cmd << " is not a notebox command. See 'help'." << std::endl;
            auto suggestion = findClosestCommand(cmd, factory.list_commands());
            if (suggestion.has_value())
            {
                std::cout << "The most similar command is" << std::endl << "        " << suggestion.value() << std::endl;
                bool use_suggested_command = NoteBox::Utils::ask_yes_no("Do you want to use the command " + suggestion.value() + "?");
                if (use_suggested_command)
                {
                    command = factory.getCommand(suggestion.value());
                    command->execute(note_box_manager, args);
                }

            }
        }

        // Show path after the command
        // std::cout << ":" << note_box_manager.note_manager.pwd() << "\n";
    }

    auto session = note_box_manager.session_manager.get();
    session.last_opened = NoteBox::Utils::currentUnixTimestamp();
    note_box_manager.session_manager.update(session);

    return 0;
}
