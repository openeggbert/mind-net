//
// Created by robertvokac on 9/2/25.
//
#pragma once


#include <memory>

#include "mindnet/essential/DatabaseType.hpp"
#include "mindnet/api/ValidatorBase.hpp"
#include "mindnet/plugins/slipbox/models/Task.hpp"

namespace mindnet::plugins::slipbox::validators
{
    using api::RequestContext;
    using mindnet::plugins::slipbox::models::Task;

    class TaskValidator : public api::ValidatorBase<
            TaskValidator, Task>
    {
    public:
        TaskValidator() = default;
        ~TaskValidator() = default; // explicitly make it destructible
        using Model = Task;

        create_method_prototypes_for_ValidatorBase(Model)

        // Additional validation methods could be added here if needed:
        // - Validate progress is between 0-100
        // - Validate status values
        // - Validate date relationships (start_date, due_date, completed_at)
        // - Validate task dependencies (parent_task_id, blocked_by_task_id)
    };
}
