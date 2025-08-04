//
// Created by robertvokac on 7/31/25.
//
#ifndef ACTIONITEM_H
#define ACTIONITEM_H

#include <NoteBox/Menu/MenuItem.h>
#include <functional>

#include "SubMenu.h"

namespace NoteBox::Menu {
    class ActionItem : public MenuItem {
    public:
        ActionItem(const std::string &title, const std::function<void()> &action);

    public:
        void execute() override;

        static std::shared_ptr<ActionItem> create(const std::shared_ptr<SubMenu> &parentSubMenu, const std::string &title,
                                                  const std::function<void()> &action);

    private:
        std::function<void()> action;
    };
} // ActionItem::NoteBox
#endif // ACTIONITEM_H
