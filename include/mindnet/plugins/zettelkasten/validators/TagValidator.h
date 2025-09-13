//
// Created by robertvokac on 9/2/25.
//
#ifndef TAGVALIDATOR_H
#define TAGVALIDATOR_H

#include <memory>

#include "mindnet/Helper.h"
#include "../../../api/ValidatorBase.h"

namespace mindnet::plugins::zettelkasten::validators
{
    using api::RequestContext;
    using mindnet::plugins::zettelkasten::models::Tag;

    class TagValidator : public api::ValidatorBase<TagValidator, Tag>
    {
    public:
        TagValidator() = default;
        ~TagValidator() = default; // explicitly make it destructible
        using Model = Tag;

        create_method_prototypes_for_ValidatorBase(Tag)
    };
}
#endif // TAGVALIDATOR_H
