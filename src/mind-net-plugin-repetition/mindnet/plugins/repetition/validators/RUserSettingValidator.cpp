//
// Created by robertvokac on 8/6/25.
//

#include "mindnet/plugins/repetition/validators/RUserSettingValidator.h"

#include "mindnet/essential/Global.h"
#include "mindnet/plugins/core/enums/SingleRight.h"
#include "mindnet/plugins/repetition/models/RUserSetting.h"
#include "mindnet/api/Persistence.h"

#define Model RUserSetting
#define MODEL R_USER_SETTING
#define model r_user_setting

namespace mindnet::plugins::repetition::validators
{
    using validators::RUserSettingValidator;
    using mindnet::api::OperationResult;
    using mindnet::essential::g_configuration;

    OperationResult RUserSettingValidator::validate_create_authorization(
        const RequestContext& ctx, const Model& entity) const
    {
        // Users can only create their own settings
        if (ctx.user_id != entity.user_id)
        {
            return {403, "Can only create settings for yourself"};
        }
        return ok_result;
    }

    OperationResult RUserSettingValidator::validate_read_authorization(const RequestContext& ctx,
                                                                       const Model& entity) const
    {
        // Users can only read their own settings
        if (ctx.user_id != entity.user_id)
        {
            return {403, "Can only read your own settings"};
        }
        return ok_result;
    }

    OperationResult RUserSettingValidator::validate_update_authorization(
        const RequestContext& ctx, const Model& old_entity,
        const Model& new_entity) const
    {
        // Users can only update their own settings
        if (ctx.user_id != new_entity.user_id)
        {
            return {403, "Can only update your own settings"};
        }
        return ok_result;
    }

    OperationResult RUserSettingValidator::validate_delete_authorization(
        const RequestContext& ctx, const Model& entity) const
    {
        // Users can only delete their own settings
        if (ctx.user_id != entity.user_id)
        {
            return {403, "Can only delete your own settings"};
        }
        return ok_result;
    }

    OperationResult RUserSettingValidator::validate_list_authorization(const RequestContext& ctx,
                                                                       const string_map& filter) const
    {
        // Users can only list their own settings
        auto it = filter.find("user_id");
        if (it == filter.end() || std::stoi(it->second) != ctx.user_id)
        {
            return {403, "Can only list your own settings"};
        }
        return ok_result;
    }

    OperationResult RUserSettingValidator::validate_create_integrity(const RequestContext& ctx,
                                                                     const Model& entity) const
    {
        if (entity.key.empty())
        {
            return {400, "Setting key cannot be empty"};
        }
        if (entity.value.empty())
        {
            return {400, "Setting value cannot be empty"};
        }
        return ok_result;
    }

    OperationResult RUserSettingValidator::validate_read_integrity(const RequestContext& ctx, const Model& entity) const
    {
        return ok_result;
    }

    OperationResult RUserSettingValidator::validate_update_integrity(const RequestContext& ctx, const Model& old_entity,
                                                                     const Model& new_entity) const
    {
        if (new_entity.key.empty())
        {
            return {400, "Setting key cannot be empty"};
        }
        if (new_entity.value.empty())
        {
            return {400, "Setting value cannot be empty"};
        }
        return ok_result;
    }

    OperationResult RUserSettingValidator::validate_delete_integrity(const RequestContext& ctx,
                                                                     const Model& entity) const
    {
        return ok_result;
    }

    OperationResult RUserSettingValidator::validate_list_integrity(const RequestContext& ctx,
                                                                   const string_map& filter) const
    {
        return ok_result;
    }

    string RUserSettingValidator::get_model_name() const
    {
        return STRINGIFY(model);
    }
}

#undef Model
#undef MODEL
#undef model
