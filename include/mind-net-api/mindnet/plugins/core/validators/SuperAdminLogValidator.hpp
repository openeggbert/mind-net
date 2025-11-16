//
// Created by robertvokac on 9/21/25.
//
#pragma once


#include <memory>

#include "mindnet/essential/DatabaseType.hpp"
#include "mindnet/api/ValidatorBase.hpp"
#include "mindnet/plugins/core/models/SuperAdminLog.hpp"

namespace mindnet::plugins::core::validators
{
    using api::RequestContext;
    using mindnet::plugins::core::models::SuperAdminLog;

    class SuperAdminLogValidator : public api::ValidatorBase<SuperAdminLogValidator, SuperAdminLog>
    {
    public:
        SuperAdminLogValidator() = default;
        ~SuperAdminLogValidator() = default; // explicitly make it destructible
        using Model = SuperAdminLog;

        create_method_prototypes_for_ValidatorBase(Model)
    };
}
