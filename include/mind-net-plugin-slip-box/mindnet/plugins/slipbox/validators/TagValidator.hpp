//
// Created by robertvokac on 9/2/25.
//
#ifndef TAGVALIDATOR_H
#define TAGVALIDATOR_H

#include <memory>

#include "mindnet/essential/DatabaseType.hpp"
#include "mindnet/api/ValidatorBase.hpp"
#include "mindnet/plugins/slipbox/models/Tag.hpp"

namespace mindnet::plugins::slipbox::validators
{
    using api::RequestContext;
    using mindnet::plugins::slipbox::models::Tag;

    class TagValidator : public api::ValidatorBase<TagValidator, Tag>
    {
    public:
        TagValidator() = default;
        ~TagValidator() = default; // explicitly make it destructible
        using Model = Tag;

        create_method_prototypes_for_ValidatorBase(Model)
    };
}
#endif // TAGVALIDATOR_H
