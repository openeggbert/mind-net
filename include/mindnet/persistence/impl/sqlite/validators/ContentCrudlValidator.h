//
// Created by robertvokac on 9/2/25.
//
#ifndef CONTENTCRUDLVALIDATOR_H
#define CONTENTCRUDLVALIDATOR_H


#include <memory>

#include "mindnet/Helper.h"
#include "mindnet/persistence/api/CrudlValidator.h"

namespace mindnet::persistence
{
    class Persistence;
}

namespace mindnet::persistence::impl::sqlite::validators
{
    using db_ = mindnet::persistence::Persistence*;

    class ContentCrudlValidator : public api::CrudlValidator
    {
    public:
        ContentCrudlValidator() = default;
        ~ContentCrudlValidator() = default; // explicitly make it destructible
        create_h_methods()

    };

}
#endif // CONTENTCRUDLVALIDATOR_H
