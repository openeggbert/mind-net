//
// Created by robertvokac on 9/2/25.
//
#ifndef TAGTYPECRUDLVALIDATOR_H
#define TAGTYPECRUDLVALIDATOR_H


#include <memory>

#include "mindnet/Helper.h"
#include "mindnet/persistence/api/CrudlValidatorBase.h"

namespace mindnet::persistence
{
    class Persistence;
}

namespace mindnet::persistence::impl::sqlite::validators
{
    using db_ = mindnet::persistence::Persistence*;
    using api::ValidatorContext;
    using models::TagType;

    class TagTypeCrudlValidator : public api::CrudlValidatorBase<TagTypeCrudlValidator, models::TagType>
    {
    public:
        TagTypeCrudlValidator() = default;
        ~TagTypeCrudlValidator() = default; // explicitly make it destructible
        using Model = TagType;

        create_method_prototypes_for_CrudlValidatorBase(TagType)

    };

}
#endif // TAGTYPECRUDLVALIDATOR_H
