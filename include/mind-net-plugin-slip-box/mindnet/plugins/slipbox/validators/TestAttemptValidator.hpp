// Created by robertvokac on 9/2/25.
//
#pragma once

#include <memory>

#include "mindnet/essential/DatabaseType.hpp"
#include "mindnet/api/ValidatorBase.hpp"
#include "mindnet/plugins/slipbox/models/TestAttempt.hpp"

namespace mindnet::plugins::slipbox::validators
{
    using api::RequestContext;
    using mindnet::plugins::slipbox::models::TestAttempt;

    class TestAttemptValidator : public api::ValidatorBase<TestAttemptValidator, TestAttempt>
    {
    public:
        TestAttemptValidator() = default;
        ~TestAttemptValidator() = default;

        using Model = TestAttempt;

        create_method_prototypes_for_ValidatorBase(Model)
    };
}
