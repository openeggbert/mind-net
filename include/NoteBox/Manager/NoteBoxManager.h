//
// Created by robertvokac on 8/6/25.
//

#ifndef NOTEBOX_H
#define NOTEBOX_H
#include "LiteratureSourceManager.h"
#include "NoteManager.h"
#include "SessionManager.h"


namespace NoteBox::Persistence
{
    class DB;
}

namespace NoteBox::Manager {
    class NoteBoxManager {
    public:
        LiteratureSourceManager literature_source_manager;
        NoteManager note_manager;
        SessionManager session_manager;

        NoteBoxManager(const std::shared_ptr<Persistence::DB>& db);
    };

}


#endif //NOTEBOX_H
