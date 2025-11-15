//
// Created by robertvokac on 9/2/25.
//
#ifndef FLAGVALIDATOR_H
#define FLAGVALIDATOR_H

#include <memory>

#include "mindnet/essential/DatabaseType.hpp"
#include "mindnet/api/ValidatorBase.hpp"
#include "mindnet/plugins/slipbox/models/Flag.hpp"

namespace mindnet::plugins::slipbox::validators
{
    using api::RequestContext;
    using mindnet::plugins::slipbox::models::Flag;

    class FlagValidator : public api::ValidatorBase<
            FlagValidator, Flag>
    {
    public:
        FlagValidator() = default;
        ~FlagValidator() = default; // explicitly make it destructible
        using Model = Flag;

        create_method_prototypes_for_ValidatorBase(Model)
    };
}
#endif // FLAGVALIDATOR_H
