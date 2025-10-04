//
// Created by robertvokac on 9/2/25.
//
#ifndef IDEAVALIDATOR_H
#define IDEAVALIDATOR_H

#include <memory>

#include "mindnet/essential/Helper.h"
#include "mindnet/api/ValidatorBase.h"
#include "mindnet/plugins/slipbox/models/Idea.h"

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
#endif // IDEAVALIDATOR_H
