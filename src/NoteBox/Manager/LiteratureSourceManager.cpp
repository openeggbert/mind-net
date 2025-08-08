//
// Created by robertvokac on 8/4/25.
//

#include "NoteBox/Manager/LiteratureSourceManager.h"

#include <iostream>

#include "NoteBox/Utils.h"

namespace NoteBox::Manager
{
    LiteratureSourceManager::LiteratureSourceManager(const std::shared_ptr<Persistence::DB>& db_) : db(db_)
    {
    }

    void LiteratureSourceManager::create()
    {
        using std::string;
        typedef string s;
        s title;
        s author;
        ushort year = 0;
        s publication;
        s url;
        s content;
        using std::cout;
        using std::cin;
        using std::endl;
        cout << "Title: ";
        getline(cin,title);cin.clear();
        cout << "Author: ";
        getline(cin,author);cin.clear();
        cout << "Year: ";
        string year_str;
        int year_attempt = 0;
        while (true) {
        getline(cin,year_str);cin.clear();
        try
        {
            year = std::stoi(year_str);
            break;
        } catch (std::invalid_argument& e)
        {
            std::cerr << "Invalid year, try again: " << year_str << std::endl;
            year_attempt++;
            if (year_attempt > 3) {
                std::cerr << "Too many attempts, aborting." << std::endl;
                return;
            }
        }
    }
        cout << "Publication: ";
        getline(cin,publication);cin.clear();
        cout << "Url: ";
        getline(cin,url);cin.clear();
        cin.clear();
        content = Utils::editTextInEditor("", "featherpad");
        if (!NoteBox::Utils::ask_yes_no("Do you really want to create this new LiteratureSource?")) {return;}
        Entity::LiteratureSource literature_source = {0, title, author, year, publication, url, content};

        db->literature_source_repository->create(literature_source);
    }

    std::vector<Entity::LiteratureSource> LiteratureSourceManager::list()
    {
        std::string title_like = std::string("");
        return db->literature_source_repository->list(title_like);
    }

    void LiteratureSourceManager::remove(const Entity::LiteratureSource& literature_source)
    {
        db->literature_source_repository->remove(literature_source);
    }

    void LiteratureSourceManager::update(Entity::LiteratureSource& literature_source)
    {
        db->literature_source_repository->update(literature_source);
    }
}
