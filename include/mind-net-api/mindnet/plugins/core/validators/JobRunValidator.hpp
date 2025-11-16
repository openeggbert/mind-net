//
// Created by robertvokac on 9/2/25.
//
#pragma once


#include <memory>

#include "mindnet/api/ValidatorBase.hpp"
#include "mindnet/plugins/core/models/JobRun.hpp"

namespace mindnet::plugins::core::validators
{
    using api::RequestContext;
    using mindnet::plugins::core::models::JobRun;

    class JobRunValidator : public api::ValidatorBase<JobRunValidator, JobRun>
    {
    public:
        JobRunValidator() = default;
        ~JobRunValidator() = default; // explicitly make it destructible
        using Model = JobRun;

        create_method_prototypes_for_ValidatorBase(Model)
    };
}
