//
// Created by robertvokac on 8/6/25.
//

#include "NoteBox/Manager/NoteBox.h"



namespace NoteBox::Manager
{
    NoteBox::NoteBox(const std::shared_ptr<Persistence::DB>& db)
        : notes(db)
    //, tags(db), collections(db), references(db)
    {
    }
    ;
}
