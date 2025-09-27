//
// Created by robertvokac on 9/2/25.
//
#ifndef PROJECTVALIDATOR_H
#define PROJECTVALIDATOR_H

#include <memory>

#include "mindnet/essential/Helper.h"
#include "mindnet/api/ValidatorBase.h"
#include "mindnet/plugins/slipbox/models/Project.h"

namespace mindnet::plugins::slipbox::validators
{
    using api::RequestContext;
    using mindnet::plugins::slipbox::models::Project;

    class ProjectValidator : public api::ValidatorBase<
            ProjectValidator, Project>
    {
    public:
        ProjectValidator() = default;
        ~ProjectValidator() = default; // explicitly make it destructible
        using Model = Project;

        create_method_prototypes_for_ValidatorBase(Project)

        // Additional validation methods could be added here if needed:
        // - Validate progress is between 0 and 100
        // - Validate date relationships (created_at, updated_at, due_date)
        // - Validate required fields (title)
        // - Validate foreign key references (note_id, created_by, owner_id, assigned_to)
        // - Validate unique constraints (note_id + title combination)
    };
}
#endif // PROJECTVALIDATOR_H
