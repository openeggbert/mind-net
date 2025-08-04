//
// Created by robertvokac on 7/31/25.
//

#ifndef MENUITEM_H
#define MENUITEM_H

#include <string>

namespace NoteBox::Menu {
    class MenuItem {
    public:
        MenuItem(const std::string &title) : title(title) {
        }

        virtual ~MenuItem() = default;

        virtual void execute() = 0;

        virtual std::string getTitle() const { return title; }

    protected:
        std::string title;
    };
}

#endif //MENUITEM_H
