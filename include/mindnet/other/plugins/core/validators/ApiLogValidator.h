//
// Created by robertvokac on 9/21/25.
//
#ifndef APILOGVALIDATOR_H
#define APILOGVALIDATOR_H

#include <memory>

#include "mindnet/core/Helper.h"
#include "../../../api/ValidatorBase.h"
#include "mindnet/other/plugins/core/models/ApiLog.h"

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

        create_method_prototypes_for_ValidatorBase(ApiLog)
    };
}
#endif // APILOGVALIDATOR_H
