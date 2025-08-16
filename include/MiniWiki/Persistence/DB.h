//
// Created by robertvokac on 8/6/25.
//
#ifndef DB_H
#define DB_H

#include "Api/ContentRepository.h"
#include "Api/IdeaRepository.h"
#include "Api/LiteratureSourceRepository.h"
#include "Api/NoteRepository.h"
#include "Api/OldContentRepository.h"
#include "Api/OldEntityRepository.h"
#include "Api/SessionRepository.h"
#include "Api/TermRepository.h"


namespace MiniWiki::Persistence {
    class DB {
    public:
        Api::LiteratureSourceRepository* literature_source_repository{};
        Api::SessionRepository* session_repository{};
        Api::ContentRepository* content_repository{};
        Api::OldContentRepository* old_content_repository{};
        Api::OldEntityRepository* old_entity_repository{};
        Api::NoteRepository* note_repository{};
        Api::TermRepository* term_repository{};
        Api::IdeaRepository* idea_repository{};

        DB();
    };

}
#endif // DB_H
