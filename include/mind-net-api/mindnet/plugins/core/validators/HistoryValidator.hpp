//
// Created by robertvokac on 9/2/25.
//
#pragma once


#include <memory>

#include "mindnet/essential/DatabaseType.hpp"
#include "mindnet/api/ValidatorBase.hpp"
#include "mindnet/plugins/core/models/History.hpp"

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

        create_method_prototypes_for_ValidatorBase(Model)
    };
}
