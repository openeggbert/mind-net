//
// Created by robertvokac on 9/2/25.
//
#ifndef TAGTYPEVALIDATOR_H
#define TAGTYPEVALIDATOR_H

#include <memory>

#include "mindnet/essential/DatabaseType.hpp"
#include "mindnet/api/ValidatorBase.hpp"
#include "mindnet/plugins/slipbox/models/TagType.hpp"

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

        create_method_prototypes_for_ValidatorBase(Model)
    };
}
#endif // TAGTYPEVALIDATOR_H
