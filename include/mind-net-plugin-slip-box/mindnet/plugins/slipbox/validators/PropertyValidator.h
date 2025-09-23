//
// Created by robertvokac on 9/2/25.
//
#ifndef PROPERTYVALIDATOR_H
#define PROPERTYVALIDATOR_H

#include <memory>

#include "mindnet/essential/Helper.h"
#include "mindnet/api/ValidatorBase.h"
#include "mindnet/plugins/slipbox/models/Property.h"

namespace mindnet::plugins::slipbox::validators
{
    using api::RequestContext;
    using mindnet::plugins::slipbox::models::Property;

    class PropertyValidator : public api::ValidatorBase<PropertyValidator, Property>
    {
    public:
        PropertyValidator() = default;
        ~PropertyValidator() = default; // explicitly make it destructible
        using Model = Property;

        create_method_prototypes_for_ValidatorBase(Property)
    };
}
#endif // PROPERTYVALIDATOR_H
