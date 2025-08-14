//
// Created by robertvokac on 8/4/25.
//

#include "NoteBox/Entity/Idea.h"

namespace NoteBox::Entity
{
    using Persistence::Impl::Sqlite::Tables::IdeaTable;

    entity_fields Idea::get_entity_fields() const
    {
        entity_fields result;
        result.push_back(id);
        result.push_back(text);
        result.push_back(category);
        result.push_back(static_cast<int64_t>(created_at));
        result.push_back(static_cast<int64_t>(due_date));
        return result;
    };
}
