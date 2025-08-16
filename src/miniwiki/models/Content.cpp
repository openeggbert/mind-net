//
// Created by robertvokac on 8/4/25.
//

#include "miniwiki/models/Content.h"

namespace miniwiki::models
{

    entity_fields Content::get_entity_fields() const
    {
        entity_fields result;
        result.push_back(id);
        result.push_back(content);
        result.push_back(format);
        result.push_back(static_cast<int64_t>(created_at));
        return result;
    };
}
