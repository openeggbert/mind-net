//
// Created by robertvokac on 9/2/25.
//
#pragma once


#include <memory>

#include "mindnet/essential/DatabaseType.hpp"
#include "mindnet/api/ValidatorBase.hpp"
#include "mindnet/plugins/repetition/models/RUserSetting.hpp"

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
