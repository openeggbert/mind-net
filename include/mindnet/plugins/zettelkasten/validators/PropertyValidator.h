//
// Created by robertvokac on 9/2/25.
//
#ifndef PROPERTYVALIDATOR_H
#define PROPERTYVALIDATOR_H

#include <memory>

#include "mindnet/Helper.h"
#include "mindnet/persistence/api/ValidatorBase.h"

namespace mindnet::plugins::zettelkasten::validators
{
    using persistence::api::RequestContext;
    using mindnet::plugins::zettelkasten::models::Property;

    class PropertyValidator : public persistence::api::ValidatorBase<PropertyValidator, Property>
    {
    public:
        PropertyValidator() = default;
        ~PropertyValidator() = default; // explicitly make it destructible
        using Model = Property;

        create_method_prototypes_for_ValidatorBase(Property)
    };
}
#endif // PROPERTYVALIDATOR_H
