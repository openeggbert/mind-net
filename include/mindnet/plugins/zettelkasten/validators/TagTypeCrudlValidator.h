//
// Created by robertvokac on 9/2/25.
//
#ifndef TAGTYPECRUDLVALIDATOR_H
#define TAGTYPECRUDLVALIDATOR_H


#include <memory>

#include "mindnet/Helper.h"
#include "mindnet/persistence/api/CrudlValidatorBase.h"


namespace mindnet::plugins::zettelkasten::validators
{
    using persistence::api::RequestContext;
    using mindnet::plugins::zettelkasten::models::TagType;

    class TagTypeCrudlValidator : public persistence::api::CrudlValidatorBase<TagTypeCrudlValidator, TagType>
    {
    public:
        TagTypeCrudlValidator() = default;
        ~TagTypeCrudlValidator() = default; // explicitly make it destructible
        using Model = TagType;

        create_method_prototypes_for_CrudlValidatorBase(TagType)
    };
}
#endif // TAGTYPECRUDLVALIDATOR_H
