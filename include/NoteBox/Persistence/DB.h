//
// Created by robertvokac on 8/6/25.
//
#ifndef DB_H
#define DB_H


#include <memory>

#include "Api/LiteratureSourceRepository.h"
#include "Api/SessionRepository.h"


namespace NoteBox::Persistence {
    class DB {
    public:
        Api::LiteratureSourceRepository* literature_source_repository{};
        Api::SessionRepository* session_repository{};

        DB();
    };

}
#endif // DB_H
