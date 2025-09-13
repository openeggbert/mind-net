//
// Created by robertvokac on 9/2/25.
//
#ifndef TAGTYPEVALIDATOR_H
#define TAGTYPEVALIDATOR_H

#include <memory>

#include "mindnet/Helper.h"
#include "mindnet/persistence/api/ValidatorBase.h"

namespace mindnet::plugins::zettelkasten::validators
{
    using persistence::api::RequestContext;
    using mindnet::plugins::zettelkasten::models::TagType;

    class TagTypeValidator : public persistence::api::ValidatorBase<TagTypeValidator, TagType>
    {
    public:
        TagTypeValidator() = default;
        ~TagTypeValidator() = default; // explicitly make it destructible
        using Model = TagType;

        create_method_prototypes_for_ValidatorBase(TagType)
    };
}
#endif // TAGTYPEVALIDATOR_H
