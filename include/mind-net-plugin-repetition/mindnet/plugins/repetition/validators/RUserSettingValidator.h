//
// Created by robertvokac on 9/2/25.
//
#ifndef RUSERSETTINGVALIDATOR_H
#define RUSERSETTINGVALIDATOR_H

#include <memory>

#include "mindnet/essential/Helper.h"
#include "mindnet/api/ValidatorBase.h"
#include "mindnet/plugins/repetition/models/RUserSetting.h"

namespace mindnet::plugins::repetition::validators
{
    using api::RequestContext;
    using mindnet::plugins::repetition::models::RUserSetting;

    class RUserSettingValidator : public api::ValidatorBase<RUserSettingValidator, RUserSetting>
    {
    public:
        RUserSettingValidator() = default;
        ~RUserSettingValidator() = default; // explicitly make it destructible
        using Model = RUserSetting;

        create_method_prototypes_for_ValidatorBase(Model)
    };
}
#endif // RUSERSETTINGVALIDATOR_H
