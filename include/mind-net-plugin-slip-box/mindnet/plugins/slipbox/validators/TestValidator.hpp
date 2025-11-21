// Created by robertvokac on 9/2/25.
//
#pragma once

#include <memory>

#include "mindnet/essential/DatabaseType.hpp"
#include "mindnet/api/ValidatorBase.hpp"
#include "mindnet/plugins/slipbox/models/Test.hpp"

namespace mindnet::plugins::slipbox::validators
{
    using api::RequestContext;
    using mindnet::plugins::slipbox::models::Test;

    class TestValidator : public api::ValidatorBase<TestValidator, Test>
    {
    public:
        TestValidator() = default;
        ~TestValidator() = default;

        using Model = Test;

        create_method_prototypes_for_ValidatorBase(Model)
    };
}
