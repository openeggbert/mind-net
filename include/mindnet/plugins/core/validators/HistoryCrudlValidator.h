//
// Created by robertvokac on 9/2/25.
//
#ifndef HISTORYCRUDLVALIDATOR_H
#define HISTORYCRUDLVALIDATOR_H


#include <memory>

#include "mindnet/Helper.h"
#include "mindnet/persistence/api/CrudlValidatorBase.h"


namespace mindnet::plugins::core::validators
{

    using persistence::api::RequestContext;
    using mindnet::plugins::core::models::History;

    class HistoryCrudlValidator : public persistence::api::CrudlValidatorBase<HistoryCrudlValidator, History>
    {
    public:
        HistoryCrudlValidator() = default;
        ~HistoryCrudlValidator() = default; // explicitly make it destructible
        using Model = History;

        create_method_prototypes_for_CrudlValidatorBase(History)

    };

}
#endif // HISTORYCRUDLVALIDATOR_H
