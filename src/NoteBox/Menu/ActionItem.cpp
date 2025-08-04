//
// Created by robertvokac on 7/31/25.
//

#include "NoteBox/Menu/ActionItem.h"

namespace NoteBox::Menu {
    ActionItem::ActionItem(const std::string &title, const std::function<void()> &action)
        : MenuItem(title), action(action) {
    }

    void ActionItem::execute() {
        action();
    }

    std::shared_ptr<ActionItem> ActionItem::create(const std::shared_ptr<SubMenu>  &parentSubMenu, const std::string &title,
                                                   const std::function<void()> &action) {
        auto self = std::make_shared<ActionItem>(title, action);
        parentSubMenu->addItem(self);
        return self;
    }
} // ActionItem::NoteBox
