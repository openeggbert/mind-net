//
// Created by robertvokac on 8/6/25.
//

#ifndef NOTEBOX_H
#define NOTEBOX_H
#include "NoteManager.h"


namespace NoteBox::Manager {
    class NoteBox {
    public:
        NoteManager notes;

        NoteBox(std::shared_ptr<Persistence> db)
            : notes(db)
        //, tags(db), collections(db), references(db)
        {}
    };

}


#endif //NOTEBOX_H
