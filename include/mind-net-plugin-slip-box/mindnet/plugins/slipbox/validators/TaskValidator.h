//
// Created by robertvokac on 9/2/25.
//
#ifndef TASKVALIDATOR_H
#define TASKVALIDATOR_H

#include <memory>

#include "mindnet/essential/Helper.h"
#include "mindnet/api/ValidatorBase.h"
#include "mindnet/plugins/slipbox/models/Task.h"

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

        create_method_prototypes_for_ValidatorBase(Task)

        // Additional validation methods could be added here if needed:
        // - Validate progress is between 0-100
        // - Validate status values
        // - Validate date relationships (start_date, due_date, completed_at)
        // - Validate task dependencies (parent_task_id, blocked_by_task_id)
    };
}
#endif // TASKVALIDATOR_H
