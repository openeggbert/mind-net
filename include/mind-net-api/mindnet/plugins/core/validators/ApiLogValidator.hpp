//
// Created by robertvokac on 9/21/25.
//
#pragma once


#include <memory>

#include "mindnet/essential/DatabaseType.hpp"
#include "mindnet/api/ValidatorBase.hpp"
#include "mindnet/plugins/core/models/ApiLog.hpp"

namespace mindnet::plugins::core::validators
{
    using api::RequestContext;
    using mindnet::plugins::core::models::ApiLog;

    class ApiLogValidator : public api::ValidatorBase<ApiLogValidator, ApiLog>
    {
    public:
        ApiLogValidator() = default;
        ~ApiLogValidator() = default; // explicitly make it destructible
        using Model = ApiLog;

        create_method_prototypes_for_ValidatorBase(Model)
    };
}
