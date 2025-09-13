//
// Created by robertvokac on 9/2/25.
//
#ifndef CONTENTVALIDATOR_H
#define CONTENTVALIDATOR_H

#include <memory>

#include "mindnet/Helper.h"
#include "mindnet/persistence/api/ValidatorBase.h"

namespace mindnet::plugins::zettelkasten::validators
{
    using persistence::api::RequestContext;
    using mindnet::plugins::zettelkasten::models::Content;

    class ContentValidator : public persistence::api::ValidatorBase<ContentValidator, Content>
    {
    public:
        ContentValidator() = default;
        ~ContentValidator() = default; // explicitly make it destructible
        using Model = Content;

        create_method_prototypes_for_ValidatorBase(Content)
    };
}
#endif // CONTENTVALIDATOR_H
