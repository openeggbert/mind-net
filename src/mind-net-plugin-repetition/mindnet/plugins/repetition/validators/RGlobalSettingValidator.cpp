//
// Created by robertvokac on 8/6/25.
//

#include "mindnet/plugins/repetition/validators/RGlobalSettingValidator.h"

#include "mindnet/essential/Global.h"
#include "mindnet/plugins/core/enums/SingleRight.h"
#include "mindnet/plugins/repetition/models/RGlobalSetting.h"
#include "mindnet/api/Persistence.h"

#define Model RGlobalSetting
#define MODEL R_GLOBAL_SETTING
#define model r_global_setting

namespace mindnet::plugins::repetition::validators
{
    using validators::RGlobalSettingValidator;
    using mindnet::api::OperationResult;
    using mindnet::essential::g_configuration;

    OperationResult RGlobalSettingValidator::validate_create_authorization(
        const RequestContext& ctx, const Model& entity) const
    {
        // Only admin should be able to create global settings
        assert_admin()
        return ok_result;
    }

    OperationResult RGlobalSettingValidator::validate_read_authorization(
        const RequestContext& ctx, const Model& entity) const
    {
        // Anyone can read global settings
        return ok_result;
    }

    OperationResult RGlobalSettingValidator::validate_update_authorization(
        const RequestContext& ctx, const Model& old_entity,
        const Model& new_entity) const
    {
        // Only admin should be able to update global settings
        assert_admin()

        return ok_result;
    }

    OperationResult RGlobalSettingValidator::validate_delete_authorization(
        const RequestContext& ctx, const Model& entity) const
    {
        // Only admin should be able to delete global settings
        assert_admin()

        return ok_result;
    }

    OperationResult RGlobalSettingValidator::validate_list_authorization(const RequestContext& ctx,
                                                                         const string_map& filter) const
    {
        // Anyone can list global settings
        return ok_result;
    }

    OperationResult RGlobalSettingValidator::validate_create_integrity(const RequestContext& ctx,
                                                                       const Model& entity) const
    {
        return ok_result;
    }

    OperationResult RGlobalSettingValidator::validate_read_integrity(const RequestContext& ctx,
                                                                     const Model& entity) const
    {
        return ok_result;
    }

    OperationResult RGlobalSettingValidator::validate_update_integrity(const RequestContext& ctx,
                                                                       const Model& old_entity,
                                                                       const Model& new_entity) const
    {
        return ok_result;
    }

    OperationResult RGlobalSettingValidator::validate_delete_integrity(const RequestContext& ctx,
                                                                       const Model& entity) const
    {
        return ok_result;
    }

    OperationResult RGlobalSettingValidator::validate_list_integrity(const RequestContext& ctx,
                                                                     const string_map& filter) const
    {
        return ok_result;
    }

    string RGlobalSettingValidator::get_model_name() const
    {
        return STRINGIFY(model);
    }
}

#undef Model
#undef MODEL
#undef model
