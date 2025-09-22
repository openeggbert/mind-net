//
// Created by robertvokac on 8/4/25.
//

#include "mindnet/other/plugins/slipbox/models/Idea.h"

namespace mindnet::plugins::slipbox::models
{
    entity_fields Idea::to_values() const
    {
        entity_fields result;
        result.push_back(id);
        result.push_back(cast64(created_at));
        result.push_back(cast64(updated_at));
        //
        result.push_back(cast64(user_id));
        result.push_back(title);
        result.push_back(content);
        result.push_back(category);
        result.push_back(cast64(due_at));
        result.push_back(is_important);
        result.push_back(is_public);
        result.push_back(is_pinned);
        return result;
    }

    void Idea::from_values(const entity_fields& values)
    {
        int i = 0;

        def_helper_lambdas()

        set_id(number());
        created_at = number();
        updated_at = number();
        //
        user_id = number();
        title = text();
        content = text();
        category = text();
        due_at = number();
        is_important = boolean();
        is_public = boolean();
        is_pinned = boolean();
    }

    string Idea::validate()
    {
        using columns::IdeaColumns;

        validator_chain_vector list{
            [this] { return test_ne(user_id, 0, IdeaColumns::USER_ID); },
            [this] { return testt_not_empty(title, IdeaColumns::TITLE); },
            [this] { return testt_at_most(content, 1024, IdeaColumns::CONTENT); },
            [this] { return testt_at_most(category, 32, IdeaColumns::CATEGORY); },
            [this] { return test_at_least(due_at, 0, IdeaColumns::DUE_AT); },

        };
        return util::ValidatorChain::run(list);
    }
}
