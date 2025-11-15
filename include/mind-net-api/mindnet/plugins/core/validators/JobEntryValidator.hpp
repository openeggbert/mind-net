//
// Created by robertvokac on 9/2/25.
//
#ifndef JOBENTRYVALIDATOR_HPP
#define JOBENTRYVALIDATOR_HPP

#include <memory>

#include "mindnet/api/ValidatorBase.hpp"
#include "mindnet/plugins/core/models/JobEntry.hpp"

namespace mindnet::plugins::core::validators
{
    using api::RequestContext;
    using mindnet::plugins::core::models::JobEntry;

    class JobEntryValidator : public api::ValidatorBase<JobEntryValidator, JobEntry>
    {
    public:
        JobEntryValidator() = default;
        ~JobEntryValidator() = default; // explicitly make it destructible
        using Model = JobEntry;

        create_method_prototypes_for_ValidatorBase(Model)
    };
}
#endif // JOBENTRYVALIDATOR_HPP
