//
// Created by robertvokac on 8/4/25.
//

#include "MiniWiki/Command/UpdateCommand.h"

#include <filesystem>
#include <iostream>
#include <sstream>

#include "MiniWiki/Global.h"
#include "MiniWiki/Enum/Difficulty.h"
#include "MiniWiki/Enum/Importance.h"
#include "MiniWiki/Manager/MiniWikiManager.h"
#include "MiniWiki/Persistence/Impl/Sqlite/Tables/NoteTable.h"

namespace MiniWiki::Command
{

    void UpdateCommand::execute(Manager::MiniWikiManager& mgr, const std::string& args)
    {
        std::string used_path;
        auto pwd = mgr.note_manager.pwd();
        if (Utils::get_note_id_from_args(pwd, args, used_path)) return;
        bool exit = false;
        while (!exit)
        {
            Utils::clearScreen();
            std::cout << R"(
Select the field to be updated:
0 exit
a Title
b Question
c Expires at
d Importance
e Difficulty
f Source id

)" << std::endl;

            std::string choice_line;
            std::getline(std::cin, choice_line);
            char choice = choice_line.empty() ? '\0' : choice_line[0];
            std::cout << "Choice: " << choice << "\n" << std::flush;
            Entity::Note note = mgr.note_manager.read_note(used_path);

            switch (choice)
            {
            case '0':
                {
                    exit = true;
                    break;
                }
            case 'a':
                {
                    std::cout << "Current title: " << note.title << std::endl;
                    std::cout << "New title: ";
                    std::string new_title;
                    getline(std::cin,new_title);
                    if (new_title.empty())
                    {
                        err << "Title cannot be empty." << std::endl;
                        continue;
                    }
                    note.title = new_title;
                }
                break;
            case 'b':
                {
                    std::cout << "Current question: " << note.question << std::endl;
                    std::cout << "New question: ";
                    std::string new_question;
                    getline(std::cin,new_question);
                    note.question = new_question;
                }
                break;
            case 'c':
                {
                    std::cout << "Current expires at: " << Utils::unixToFormattedString(note.expires_at) << std::endl;
                    std::cout << "New expires at (use format yyyyMMdd): ";
                    std::string new_expires_at;
                    std::cin >> new_expires_at;
                    unixtime new_expires_at_unixtime = -1;
                    if (new_expires_at.empty())
                    {
                        new_expires_at_unixtime = 0;
                    }
                    else
                    {
                        if (new_expires_at.size() != 8)
                        {
                            err << "Invalid format. Use format yyyyMMdd" << std::endl;
                        }
                        else
                        {
                            bool all_is_digit = true;
                            for (char ch : new_expires_at)
                            {
                                if (!isdigit(ch))
                                {
                                    err << "Invalid format. Has to be digit. Use format yyyyMMdd" << std::endl;
                                    all_is_digit = false;
                                    break;
                                }
                            }
                            if (all_is_digit)
                            {
                                int year = std::stoi(new_expires_at.substr(0, 4));
                                int month = std::stoi(new_expires_at.substr(4, 2));
                                int day = std::stoi(new_expires_at.substr(6, 2));

                                std::tm timeinfo = {};
                                timeinfo.tm_year = year - 1900; // tm_year is years since 1900
                                timeinfo.tm_mon = month - 1; // tm_mon is 0-based
                                timeinfo.tm_mday = day;
                                timeinfo.tm_hour = 0;
                                timeinfo.tm_min = 0;
                                timeinfo.tm_sec = 0;
                                timeinfo.tm_isdst = -1; // Let mktime determine DST

                                new_expires_at_unixtime = static_cast<unixtime>(std::mktime(&timeinfo));
                            }
                        }
                    }

                    if (new_expires_at_unixtime >= 0) { note.expires_at = new_expires_at_unixtime; };
                }
                break;


            case 'd':
                {
                    std::cout << "Current importance: " << Enum::importance_to_string(note.importance) << std::endl;
                    std::cout << "New importance: " << std::endl;
                    std::cout << "1. Low" << std::endl;
                    std::cout << "2. Medium" << std::endl;
                    std::cout << "3. High" << std::endl << std::endl;
                    std::cout << "New importance: " << std::endl;
                    char new_importance;
                    std::cin >> new_importance;
                    std::cout << new_importance << std::endl;
                    std::cout << "Press any key to continue." << std::endl;
                    std::cin.get();

                    int min = static_cast<int>(Enum::Importance::LOW);
                    int max = static_cast<int>(Enum::Importance::HIGH);
                    std::string str;
                    str+=new_importance;
                    int new_importance_int = stoi(str);

                    if (new_importance_int < min || new_importance_int > max)
                    {
                        err << "Invalid choice" << std::endl;
                        std::cout << "Press any key to continue." << std::endl;
                        std::cin.get();
                        continue;
                    }
                    note.importance = new_importance_int;
                }

                break;

            case 'e':
                {
                    std::cout << "Current difficulty: " << Enum::difficulty_to_string(note.difficulty) << std::endl;
                    std::cout << "New difficulty: " << std::endl;
                    std::cout << "1. Low" << std::endl;
                    std::cout << "2. Medium" << std::endl;
                    std::cout << "3. High" << std::endl;
                    std::cout << "4. Expert" << std::endl << std::endl;
                    std::cout << "New difficulty: ";
                    char new_difficulty;
                    std::cin >> new_difficulty;

                    std::string str;
                    str+=new_difficulty;
                    int new_difficulty_int = stoi(str);


                    if (new_difficulty_int < static_cast<int>(Enum::Difficulty::EASY) || new_difficulty_int > static_cast<int>(
                        Enum::Difficulty::EXPERT))
                    {
                        err << "Invalid choice" << std::endl;
                        std::cout << "Press any key to continue." << std::endl;
                        std::cin.get();
                        continue;
                    }
                    note.difficulty = new_difficulty_int;
                }

                break;
            case 'f':
                {
                    ushort current_source_id = note.source_id;

                    std::string source_title;
                    if (current_source_id != 0)
                    {
                        Entity::LiteratureSource source = mgr.literature_source_manager.read(current_source_id);
                        source_title = source.title;
                    }
                    if (source_title.empty())
                    {
                        source_title = "None";
                    }
                    std::cout << "Current source: " << source_title << std::endl;
                    std::cout << "New source id: ";
                    int new_source_id = 0;
                    std::cin >> new_source_id;
                    if (new_source_id < 0)
                    {
                        err << "Invalid source id" << std::endl;
                        std::cout << "Press any key to continue." << std::endl;
                        std::cin.get();
                        continue;
                    }
                    if (new_source_id > 0)
                    {
                        try {
                        auto source = mgr.literature_source_manager.read(new_source_id);
                        } catch (std::exception& e) {err << "Invalid source id" << std::endl; continue;}
                    }


                    note.source_id = new_source_id;
                }

                break;

            default: err << "Invalid choice" << std::endl;
                continue;
            }



            Entity::Note original_note = mgr.note_manager.read_note(used_path);
            Entity::OldEntity old_entity;
            old_entity.entity_name = Persistence::Impl::Sqlite::Tables::NoteTable::TABLE_NAME;
            old_entity.entity_id = original_note.id;



            std::ostringstream oss;
            oss << note;
            std::string output = oss.str();
            old_entity.value = output;
            old_entity.added_at = Utils::currentUnixTimestamp();



            mgr.create_old_entity(old_entity);
            std::cout << "Updated note: " << note << std::endl;
            note.updated_at = Utils::currentUnixTimestamp();
            mgr.note_manager.update_note(note);
            if (exit || !Utils::ask_yes_no("Do you want to update another field?"))
            {
                break;
            }
        }


    }

    void UpdateCommand::help()
    {
        std::cout << R"(
NAME
       update

SYNOPSIS
       update [NOTE_ID]

DESCRIPTION
       Updates the information about a Note ID.

EXAMPLES
       update ... updates the information of the current Note ID.
       update 3kd43 ... updates the information of the Note with ID 3kd43.

)" << std::endl;
    }
}
