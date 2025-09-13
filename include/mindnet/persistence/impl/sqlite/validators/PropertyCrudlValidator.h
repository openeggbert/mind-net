//
// Created by robertvokac on 9/2/25.
//
#ifndef PROPERTYCRUDLVALIDATOR_H
#define PROPERTYCRUDLVALIDATOR_H


#include <memory>

#include "mindnet/Helper.h"
#include "mindnet/persistence/api/CrudlValidatorBase.h"

//namespace mindnet::persistence
//{
//    class Persistence;
//}

namespace mindnet::persistence::impl::sqlite::validators
{
    //using db_ = mindnet::persistence::Persistence*;
    using api::RequestContext;
    using mindnet::plugins::zettelkasten::models::Property;

    class PropertyCrudlValidator : public api::CrudlValidatorBase<PropertyCrudlValidator, Property>
    {
    public:
        PropertyCrudlValidator() = default;
        ~PropertyCrudlValidator() = default; // explicitly make it destructible
        using Model = Property;

        create_method_prototypes_for_CrudlValidatorBase(Property)

    };

}
#endif // PROPERTYCRUDLVALIDATOR_H
