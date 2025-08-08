//
// Created by robertvokac on 8/6/25.
//

#include "NoteBox/Manager/NoteBoxManager.h"



namespace NoteBox::Manager
{
    NoteBoxManager::NoteBoxManager(const std::shared_ptr<Persistence::DB>& db)
        :
    literature_source_manager(db),
    note_manager(db),
    session_manager(db)
    //, tags(db), collections(db), references(db)
    {
    }
    ;
}
