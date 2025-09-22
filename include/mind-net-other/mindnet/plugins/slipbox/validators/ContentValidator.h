//
// Created by robertvokac on 9/2/25.
//
#ifndef CONTENTVALIDATOR_H
#define CONTENTVALIDATOR_H

#include <memory>

#include "mindnet/essential/Helper.h"
#include "../../../api/ValidatorBase.h"

namespace mindnet::plugins::slipbox::validators
{
    using api::RequestContext;
    using mindnet::plugins::slipbox::models::Content;

    class ContentValidator : public api::ValidatorBase<ContentValidator, Content>
    {
    public:
        ContentValidator() = default;
        ~ContentValidator() = default; // explicitly make it destructible
        using Model = Content;

        create_method_prototypes_for_ValidatorBase(Content)
    };
}
#endif // CONTENTVALIDATOR_H
