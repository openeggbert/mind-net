//
// Created by robertvokac on 9/2/25.
//
#ifndef HISTORYVALIDATOR_H
#define HISTORYVALIDATOR_H

#include <memory>

#include "mindnet/core/Helper.h"
#include "../../../api/ValidatorBase.h"

namespace mindnet::plugins::core::validators
{
    using api::RequestContext;
    using mindnet::plugins::core::models::History;

    class HistoryValidator : public api::ValidatorBase<HistoryValidator, History>
    {
    public:
        HistoryValidator() = default;
        ~HistoryValidator() = default; // explicitly make it destructible
        using Model = History;

        create_method_prototypes_for_ValidatorBase(History)
    };
}
#endif // HISTORYVALIDATOR_H
