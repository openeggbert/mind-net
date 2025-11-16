//
// Created by robertvokac on 9/2/25.
//
#pragma once


#include <memory>

#include "mindnet/essential/DatabaseType.hpp"
#include "mindnet/api/ValidatorBase.hpp"
#include "mindnet/plugins/slipbox/models/Alert.hpp"

namespace mindnet::plugins::slipbox::validators
{
    using api::RequestContext;
    using mindnet::plugins::slipbox::models::Alert;

    class AlertValidator : public api::ValidatorBase<
            AlertValidator, Alert>
    {
    public:
        AlertValidator() = default;
        ~AlertValidator() = default; // explicitly make it destructible
        using Model = Alert;

        create_method_prototypes_for_ValidatorBase(Model)
    };
}
