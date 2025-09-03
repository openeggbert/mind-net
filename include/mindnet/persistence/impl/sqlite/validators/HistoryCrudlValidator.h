//
// Created by robertvokac on 9/2/25.
//
#ifndef HISTORYCRUDLVALIDATOR_H
#define HISTORYCRUDLVALIDATOR_H


#include <memory>

#include "mindnet/Helper.h"

namespace mindnet::persistence
{
    class Persistence;
}

namespace mindnet::persistence::impl::sqlite::validators
{
    using db_ = mindnet::persistence::Persistence*;

    class HistoryCrudlValidator : public api::CrudlValidator
    {
    public:
        HistoryCrudlValidator() = default;
        ~HistoryCrudlValidator() = default; // explicitly make it destructible
        create_h_methods()

    };

}
#endif // HISTORYCRUDLVALIDATOR_H
