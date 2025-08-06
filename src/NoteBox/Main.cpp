//
// Created by robertvokac on 7/31/25.
//
#include <iostream>

#include "NoteBox/Utils.h"
#include <memory>
#include <sstream>

#include "NoteBox/Global.h"
#include "NoteBox/Command/CommandFactory.h"
#include "NoteBox/Manager/NoteManager.h"

bool migrateSchemaIfNeeded() {
    std::cout <<"Migrating schema, if needed.";


    bool migrationResult = NoteBox::Persistence::Impl::Sqlite::SqliteDatabaseMigration::getInstance()->migrate();
    if (migrationResult) {
        std::cout << "Migrating schema: OK. Success." << std::endl;
        return true;
    }
    std::cerr << "Migrating schema: KO. Failed." << std::endl;
    return false;
}

int main() {
    migrateSchemaIfNeeded();
    NoteBox::Manager::NoteManager mgr;
    NoteBox::Command::CommandFactory factory;

    std::string line;
    std::cout << ":" << mgr.getCurrentPath() << "\n";
    while (std::cout << "> " && std::getline(std::cin, line)) {
        std::istringstream iss(line);
        std::string cmd, args;
        iss >> cmd;
        std::getline(iss, args);
        if (!args.empty() && args[0] == ' ') args = args.substr(1);

        if (cmd == "exit" || cmd == "quit") {
            std::cout << "Exiting application" << std::endl;
            break;
        }

        auto command = factory.getCommand(cmd);
        if (command) {
            command->execute(mgr, args);
        } else {
            std::cout << "Unknown command: " << cmd << "\n";
        }
    }
    return 0;
}
