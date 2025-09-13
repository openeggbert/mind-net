//
// Created by robertvokac on 9/2/25.
//
#ifndef DISCUSSIONCRUDLVALIDATOR_H
#define DISCUSSIONCRUDLVALIDATOR_H


#include <memory>

#include "mindnet/Helper.h"
#include "mindnet/persistence/api/CrudlValidatorBase.h"


namespace mindnet::plugins::chat::validators
{
    using persistence::api::RequestContext;
    using mindnet::plugins::chat::models::Discussion;

    class DiscussionCrudlValidator : public persistence::api::CrudlValidatorBase<DiscussionCrudlValidator, Discussion>
    {
    public:
        DiscussionCrudlValidator() = default;
        ~DiscussionCrudlValidator() = default; // explicitly make it destructible
        using Model = Discussion;

        create_method_prototypes_for_CrudlValidatorBase(Discussion)
    };
}
#endif // DISCUSSIONCRUDLVALIDATOR_H
