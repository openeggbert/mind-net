//
// Created by robertvokac on 7/31/25.
//

#include "NoteBox/Menu/SubMenu.h"

#include <iostream>

#include "NoteBox/Utils.h"
#include "NoteBox/Global.h"

namespace NoteBox::Menu {
    SubMenu::SubMenu(const std::string &title) : MenuItem(title) {
    }

    void SubMenu::addItem(const std::shared_ptr<MenuItem> &item) {
        items.push_back(item);
    }

    void SubMenu::execute() {
        while (true) {
            Utils::clearScreen();
            std::cout << "\n=== " << title << " ===\n";
            //std::cout << "Logged in as: " << NoteBox::loggedInUser << "\n";
            if (error != "") {
                std::cerr << error << "\n";
                break;
            }
            for (size_t i = 0; i < items.size(); ++i) {
                std::cout << i + 1 << ". " << items[i]->getTitle() << "\n";
            }
            if (printBackMenu) {
                std::cout << items.size() + 1 << ". Back\n";
            }
            std::cout << "\nChoice: ";

            int choice;

            std::cin >> choice;
            int choiceCount = static_cast<int>(items.size());
            if (choice >= 1 && choice <= choiceCount) {
                items[choice - 1]->execute();
            } else {
                if (printBackMenu && choice == items.size() + 1) { break; } else {
                    std::cerr << "Invalid choice.\n";
                }
            }
        }
    }

    void SubMenu::setPrintBackMenu(bool printBackMenu) { this->printBackMenu = printBackMenu; }

    void SubMenu::setError(const std::string& error) {
        this->error = error;
    }
    void SubMenu::clearError() {
        this->error = "";
    }
    std::shared_ptr<SubMenu> SubMenu::createRoot(const std::string &title) {
        auto self = std::make_shared<SubMenu>(title);
        return self;
    }

    std::shared_ptr<SubMenu> SubMenu::create(const std::shared_ptr<SubMenu> &parentSubMenu, const std::string &title) {
        auto self = createRoot(title);
        parentSubMenu->addItem(self);
        return self;
    }
}
