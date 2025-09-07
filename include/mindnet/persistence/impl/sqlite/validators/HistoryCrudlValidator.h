//
// Created by robertvokac on 9/2/25.
//
#ifndef HISTORYCRUDLVALIDATOR_H
#define HISTORYCRUDLVALIDATOR_H


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
    using api::RequestContext;
    using models::History;

    class HistoryCrudlValidator : public api::CrudlValidatorBase<HistoryCrudlValidator, models::History>
    {
    public:
        HistoryCrudlValidator() = default;
        ~HistoryCrudlValidator() = default; // explicitly make it destructible
        using Model = History;

        create_method_prototypes_for_CrudlValidatorBase(History)

    };

}
#endif // HISTORYCRUDLVALIDATOR_H
