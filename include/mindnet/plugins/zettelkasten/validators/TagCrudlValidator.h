//
// Created by robertvokac on 9/2/25.
//
#ifndef TAGCRUDLVALIDATOR_H
#define TAGCRUDLVALIDATOR_H


#include <memory>

#include "mindnet/Helper.h"
#include "mindnet/persistence/api/CrudlValidatorBase.h"


namespace mindnet::plugins::zettelkasten::validators
{

    using persistence::api::RequestContext;
    using mindnet::plugins::zettelkasten::models::Tag;

    class TagCrudlValidator : public persistence::api::CrudlValidatorBase<TagCrudlValidator, Tag>
    {
    public:
        TagCrudlValidator() = default;
        ~TagCrudlValidator() = default; // explicitly make it destructible
        using Model = Tag;

        create_method_prototypes_for_CrudlValidatorBase(Tag)

    };

}
#endif // TAGCRUDLVALIDATOR_H
