//
// Created by robertvokac on 8/4/25.
//

#include "mindnet/models/Node.h"

namespace mindnet::models
{
    entity_fields Node::get_values() const
    {
        entity_fields result;
        result.push_back(id);
        result.push_back(cast64(created_at));
        result.push_back(cast64(updated_at));
        result.push_back(uuid);
        result.push_back(map_id);
        result.push_back(sibling_position);
        result.push_back(title);
        result.push_back(content_id);
        result.push_back(parent_node_id);
        result.push_back(cast64(type));
        result.push_back(cast64(visibility));
        result.push_back(cast64(last_shown_at));
        result.push_back(cast64(expires_at));
        result.push_back(is_favorite);
        result.push_back(redirect_node_id);
        result.push_back(redirect_reason);
        result.push_back(cast64(importance));
        result.push_back(cast64(difficulty));
        return result;
    }

    void Node::from_values(const entity_fields& values)
    {
        int i = 0;

        def_helper_lambdas()

        set_id(number());
        created_at = number();
        updated_at = number();
        uuid = text();
        map_id = number();
        sibling_position = number();
        title = text();
        content_id = number();
        parent_node_id = number();
        type = static_cast<enums::NodeType>(number());
        visibility = static_cast<enums::Visibility>(number());
        last_shown_at = number();
        expires_at = number();
        is_favorite = number();
        redirect_node_id = number();
        redirect_reason = text();
        importance = static_cast<enums::Importance>(number());
        difficulty = static_cast<enums::Difficulty>(number());
    }
}
