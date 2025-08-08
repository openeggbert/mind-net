//
// Created by robertvokac on 8/6/25.
//

#ifndef NOTEBOX_H
#define NOTEBOX_H
#include "NoteManager.h"
#include "NoteBox/Persistence/Api/ConnectionFactory.h"


namespace NoteBox::Persistence
{
    class DB;
}

namespace NoteBox::Manager {
    class NoteBox {
    public:
        NoteManager notes;

        NoteBox(const std::shared_ptr<Persistence::DB>& db);
    };

}


#endif //NOTEBOX_H
