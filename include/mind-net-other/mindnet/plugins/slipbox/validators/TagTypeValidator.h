//
// Created by robertvokac on 9/2/25.
//
#ifndef TAGTYPEVALIDATOR_H
#define TAGTYPEVALIDATOR_H

#include <memory>

#include "mindnet/core/Helper.h"
#include "../../../api/ValidatorBase.h"

namespace mindnet::plugins::slipbox::validators
{
    using api::RequestContext;
    using mindnet::plugins::slipbox::models::TagType;

    class TagTypeValidator : public api::ValidatorBase<TagTypeValidator, TagType>
    {
    public:
        TagTypeValidator() = default;
        ~TagTypeValidator() = default; // explicitly make it destructible
        using Model = TagType;

        create_method_prototypes_for_ValidatorBase(TagType)
    };
}
#endif // TAGTYPEVALIDATOR_H
