//
// Created by robertvokac on 8/4/25.
//

#include "mindnet/plugins/chat/models/Comment.h"

namespace mindnet::plugins::chat::models
{
    entity_fields Comment::to_values() const
    {
        entity_fields result;
        result.push_back(id);
        result.push_back(cast64(created_at));
        result.push_back(cast64(updated_at));
        result.push_back(discussion_id);
        result.push_back(user_id);
        result.push_back(content);
        result.push_back(parent_comment_id);
        result.push_back(cast64(deleted));
        return result;
    }

    void Comment::from_values(const entity_fields& values)
    {
        int i = 0;

        def_helper_lambdas()

        set_id(number());
        created_at = number();
        updated_at = number();
        discussion_id = number();
        user_id = number();
        content = text();
        parent_comment_id = number();
        deleted = number();
    };

    string Comment::validate()
    {
        using columns::CommentColumns;

        validator_chain_vector list{
            [this] { return test_ne(discussion_id, 0, CommentColumns::DISCUSSION_ID); },
            [this] { return test_ne(user_id, 0, CommentColumns::USER_ID); },
            [this] { return testt_not_empty(content, CommentColumns::CONTENT); },
            [this] { return test_at_most(content.size(), 1000, CommentColumns::CONTENT); },
        };
        return util::ValidatorChain::run(list);
    }
}
