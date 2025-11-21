// Created by robertvokac on 9/2/25.
//
#pragma once

#include <memory>

#include "mindnet/essential/DatabaseType.hpp"
#include "mindnet/api/ValidatorBase.hpp"
#include "mindnet/plugins/slipbox/models/TestAttemptAnswer.hpp"

namespace mindnet::plugins::slipbox::validators
{
    using api::RequestContext;
    using mindnet::plugins::slipbox::models::TestAttemptAnswer;

    class TestAttemptAnswerValidator : public api::ValidatorBase<TestAttemptAnswerValidator, TestAttemptAnswer>
    {
    public:
        TestAttemptAnswerValidator() = default;
        ~TestAttemptAnswerValidator() = default;

        using Model = TestAttemptAnswer;

        create_method_prototypes_for_ValidatorBase(Model)
    };
}
