//
// Created by robertvokac on 9/2/25.
//
#ifndef LINKCRUDLVALIDATOR_H
#define LINKCRUDLVALIDATOR_H


#include <memory>

#include "mindnet/Helper.h"
#include "mindnet/persistence/api/CrudlValidatorBase.h"


namespace mindnet::plugins::zettelkasten::validators
{

    using persistence::api::RequestContext;
    using mindnet::plugins::zettelkasten::models::Link;

    class LinkCrudlValidator : public persistence::api::CrudlValidatorBase<LinkCrudlValidator, Link>
    {
    public:
        LinkCrudlValidator() = default;
        ~LinkCrudlValidator() = default; // explicitly make it destructible
        using Model = Link;

        create_method_prototypes_for_CrudlValidatorBase(Link)

    };

}
#endif // LINKCRUDLVALIDATOR_H
