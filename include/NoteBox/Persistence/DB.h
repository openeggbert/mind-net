//
// Created by robertvokac on 8/6/25.
//
#ifndef DB_H
#define DB_H


#include <memory>

#include "Api/LiteratureSourceRepository.h"


namespace NoteBox::Persistence {
    class DB {
    public:
        NoteBox::Persistence::Api::LiteratureSourceRepository* literature_source_repository{};

        DB();
    };

}
#endif // DB_H
