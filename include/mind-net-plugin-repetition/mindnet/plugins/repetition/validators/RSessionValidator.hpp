//
// Created by robertvokac on 9/2/25.
//
#ifndef RSESSION_VALIDATOR_H
#define RSESSION_VALIDATOR_H

#include <memory>

#include "mindnet/essential/DatabaseType.hpp"
#include "mindnet/api/ValidatorBase.hpp"
#include "mindnet/plugins/repetition/models/RSession.hpp"

namespace mindnet::plugins::repetition::validators
{
    using api::RequestContext;
    using mindnet::plugins::repetition::models::RSession;

    class RSessionValidator : public api::ValidatorBase<RSessionValidator, RSession>
    {
    public:
        RSessionValidator() = default;
        ~RSessionValidator() = default; // explicitly make it destructible
        using Model = RSession;

        create_method_prototypes_for_ValidatorBase(Model)
    };
}
#endif // RSESSION_VALIDATOR_H
