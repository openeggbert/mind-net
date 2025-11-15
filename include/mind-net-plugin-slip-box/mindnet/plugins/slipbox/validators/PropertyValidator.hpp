//
// Created by robertvokac on 9/2/25.
//
#ifndef PROPERTYVALIDATOR_H
#define PROPERTYVALIDATOR_H

#include <memory>

#include "mindnet/essential/DatabaseType.hpp"
#include "mindnet/api/ValidatorBase.hpp"
#include "mindnet/plugins/slipbox/models/Property.hpp"

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

        create_method_prototypes_for_ValidatorBase(Model)
    };
}
#endif // PROPERTYVALIDATOR_H
