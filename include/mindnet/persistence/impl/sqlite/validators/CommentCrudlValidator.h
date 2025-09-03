//
// Created by robertvokac on 9/2/25.
//
#ifndef COMMENTCRUDLVALIDATOR_H
#define COMMENTCRUDLVALIDATOR_H


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

    class CommentCrudlValidator : public api::CrudlValidator
    {
    public:
        CommentCrudlValidator() = default;
        ~CommentCrudlValidator() = default; // explicitly make it destructible
        create_h_methods()

    };

}
#endif // COMMENTCRUDLVALIDATOR_H
