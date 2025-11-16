//
// Created by robertvokac on 9/2/25.
//
#pragma once


#include <memory>

#include "mindnet/essential/DatabaseType.hpp"
#include "mindnet/api/ValidatorBase.hpp"
#include "mindnet/plugins/slipbox/models/Idea.hpp"

namespace mindnet::plugins::slipbox::validators
{
    using api::RequestContext;
    using mindnet::plugins::slipbox::models::Idea;

    class IdeaValidator : public api::ValidatorBase<
            IdeaValidator, Idea>
    {
    public:
        IdeaValidator() = default;
        ~IdeaValidator() = default; // explicitly make it destructible
        using Model = Idea;

        create_method_prototypes_for_ValidatorBase(Model)
    };
}
