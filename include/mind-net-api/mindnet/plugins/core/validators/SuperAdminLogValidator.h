//
// Created by robertvokac on 9/21/25.
//
#ifndef SUPERADMINLOGVALIDATOR_H
#define SUPERADMINLOGVALIDATOR_H

#include <memory>

#include "mindnet/essential/Helper.h"
#include "mindnet/api/ValidatorBase.h"
#include "mindnet/plugins/core/models/SuperAdminLog.h"

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

        create_method_prototypes_for_ValidatorBase(SuperAdminLog)
    };
}
#endif // SUPERADMINLOGVALIDATOR_H
