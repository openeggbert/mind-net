//
// Created by robertvokac on 8/4/25.
//

#include "mindnet/plugins/slipbox/models/Task.h"

namespace mindnet::plugins::slipbox::models
{
    entity_fields Task::to_values() const
    {
        entity_fields result;
        result.push_back(id);
        result.push_back(cast64(created_at));
        result.push_back(cast64(updated_at));
        result.push_back(note_id);
        result.push_back(project_id);
        result.push_back(title);
        result.push_back(description);
        result.push_back(progress);
        result.push_back(status);
        result.push_back(cast64(important));
        result.push_back(cast64(as_soon_as_possible));
        result.push_back(cast64(start_date));
        result.push_back(cast64(due_date));
        result.push_back(cast64(completed_at));
        result.push_back(created_by);
        result.push_back(owner_id);
        result.push_back(assigned_to);
        result.push_back(category);
        result.push_back(context);
        result.push_back(tags);
        result.push_back(cast64(is_public));
        result.push_back(parent_task_id);
        result.push_back(blocked_by_task_id);
        result.push_back(related_tasks);
        return result;
    }

    void Task::from_values(const entity_fields& values)
    {
        int i = 0;

        def_helper_lambdas()

        set_id(number());
        created_at = number();
        updated_at = number();
        note_id = number();
        project_id = number();
        title = text();
        description = text();
        progress = number();
        status = number();
        important = number();
        as_soon_as_possible = number();
        start_date = number();
        due_date = number();
        completed_at = number();
        created_by = number();
        owner_id = number();
        assigned_to = number();
        category = text();
        context = text();
        tags = text();
        is_public = number();
        parent_task_id = number();
        blocked_by_task_id = number();
        related_tasks = text();
    }

    string Task::validate()
    {
        using columns::TaskColumns;

        validator_chain_vector list{
            [this] { return testt_between(title, 1, 256, TaskColumns::TITLE); },
            [this] { return testt_between(description, 0, 4096, TaskColumns::DESCRIPTION); },
            [this] { return test_between(progress, 0, 100, TaskColumns::PROGRESS); },
            [this] { return test_between(status, 0, 4, TaskColumns::STATUS); }, // Assuming 5 states (0-4)
            [this] { return test_ne(created_by, 0, TaskColumns::CREATED_BY); },
            [this] { return test_ne(owner_id, 0, TaskColumns::OWNER_ID); }
        };
        return util::ValidatorChain::run(list);
    }
}
