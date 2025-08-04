//
// Created by robertvokac on 7/31/25.
//

#ifndef SUBMENU_H
#define SUBMENU_H

#include <vector>
#include <memory>

#include "MenuItem.h"

namespace NoteBox::Menu {
    class SubMenu : public MenuItem {
    public:
        SubMenu(const std::string& title);
    public:
        void addItem(const std::shared_ptr<MenuItem> &item);

        void execute() override;
        void setPrintBackMenu(bool printBackMenu);
        void setError(const std::string& error);
        void clearError();

        static std::shared_ptr<SubMenu> createRoot(const std::string& title);
        static std::shared_ptr<SubMenu> create(const std::shared_ptr<SubMenu>& parentSubMenu, const std::string& title);

    private:
        std::vector<std::shared_ptr<MenuItem>> items;
        bool printBackMenu = true;
        std::string error;
    };
}


#endif //SUBMENU_H
