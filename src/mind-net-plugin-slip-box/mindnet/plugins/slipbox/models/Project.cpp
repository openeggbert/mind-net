//
// Created by robertvokac on 8/4/25.
//

#include "mindnet/plugins/slipbox/models/Project.h"

namespace mindnet::plugins::slipbox::models
{
    entity_fields Project::to_values() const
    {
        entity_fields result;
        result.push_back(id);
        result.push_back(cast64(created_at));
        result.push_back(cast64(updated_at));
        result.push_back(note_id);
        result.push_back(title);
        result.push_back(description);
        result.push_back(progress);
        result.push_back(cast64(in_progress));
        result.push_back(cast64(important));
        result.push_back(cast64(due_date));
        result.push_back(created_by);
        result.push_back(owner_id);
        result.push_back(assigned_to);
        result.push_back(category);
        result.push_back(cast64(is_public));
        return result;
    }

    void Project::from_values(const entity_fields& values)
    {
        int i = 0;

        def_helper_lambdas()

        set_id(number());
        created_at = number();
        updated_at = number();
        note_id = number();
        title = text();
        description = text();
        progress = number();
        in_progress = number();
        important = number();
        due_date = number();
        created_by = number();
        owner_id = number();
        assigned_to = number();
        category = text();
        is_public = number();
    }

    string Project::validate()
    {
        using columns::ProjectColumns;

        validator_chain_vector list{
            [this] { return testt_not_empty(title, ProjectColumns::TITLE); },
            [this] { return test_between(progress, 0, 100, ProjectColumns::PROGRESS); },
            [this] { return test_ne(created_by, 0, ProjectColumns::CREATED_BY); },
            [this] { return test_ne(owner_id, 0, ProjectColumns::OWNER_ID); }
        };
        return util::ValidatorChain::run(list);
    }
}
