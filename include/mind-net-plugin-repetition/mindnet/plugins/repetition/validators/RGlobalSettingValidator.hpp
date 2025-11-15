//
// Created by robertvokac on 9/2/25.
//
#ifndef RGLOBALSETTINGVALIDATOR_H
#define RGLOBALSETTINGVALIDATOR_H

#include <memory>

#include "mindnet/essential/DatabaseType.hpp"
#include "mindnet/api/ValidatorBase.hpp"
#include "mindnet/plugins/repetition/models/RGlobalSetting.hpp"

namespace mindnet::plugins::repetition::validators
{
    using api::RequestContext;
    using mindnet::plugins::repetition::models::RGlobalSetting;

    class RGlobalSettingValidator : public api::ValidatorBase<RGlobalSettingValidator, RGlobalSetting>
    {
    public:
        RGlobalSettingValidator() = default;
        ~RGlobalSettingValidator() = default; // explicitly make it destructible
        using Model = RGlobalSetting;

        create_method_prototypes_for_ValidatorBase(Model)
    };
}
#endif // RGLOBALSETTINGVALIDATOR_H
