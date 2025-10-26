//
// Created by robertvokac on 8/6/25.
//

#include "mindnet/plugins/core/validators/UserValidator.h"

#include <regex>
#include "mindnet/essential/Configuration.h"
#include "mindnet/essential/Global.h"
#include "mindnet/plugins/core/models/User.h"
#include "mindnet/api/Persistence.h"

#define Model User
#define MODEL USER
#define model user

namespace mindnet::plugins::core::validators
{
    using validators::UserValidator;
    using mindnet::api::OperationResult;
    using mindnet::essential::g_configuration;

    OperationResult UserValidator::validate_create_authorization(const RequestContext& ctx, const Model& entity) const
    {
        return_if(
            g_configuration.registration_mode == mindnet::essential::RegistrationMode::AdminAddsUsers && ctx.token.ko(),
            401, "You must be logged in to create a user")

        return_if(
            g_configuration.registration_mode == mindnet::essential::RegistrationMode::AdminAddsUsers && ctx.token.ok()
            && ctx.role < mindnet::essential::UserRole::
            Admin,
            403, "You must be admin to create a user.")

        return ok_result;
    }

    OperationResult UserValidator::validate_read_authorization(const RequestContext& ctx, const Model& entity) const
    {
        return ok_result;
    }

    OperationResult UserValidator::validate_update_authorization(const RequestContext& ctx, const Model& old_entity,
                                                                 const Model& new_entity) const
    {
        bool logged_user_updates_himself = ctx.token.user_id == old_entity.get_id();

        return_if(ctx.role < mindnet::essential::UserRole::Admin && !logged_user_updates_himself,
                  403, "You can only update your own user.")

        return ok_result;
    }

    OperationResult UserValidator::validate_delete_authorization(const RequestContext& ctx, const Model& entity) const
    {
        return_if (ctx.role  < essential::UserRole::Admin, 403, "You are not allowed to delete this user");

        return ok_result;
    }

    OperationResult UserValidator::validate_list_authorization(const RequestContext& ctx,
                                                               const string_map& filter) const
    {
        return ok_result;
    }


    OperationResult UserValidator::validate_create_integrity(const RequestContext& ctx, const Model& entity) const
    {
        return_if(
            ctx.role < mindnet::essential::UserRole::Admin && entity.role != g_configuration.default_user_role && !ctx.
            token.system,
            400, "role" " must be equal to " + user_role_to_string(g_configuration.
                default_user_role))

        return_if(core::has_user_name(ctx, entity.username),
                  409, "username already exists")

        return_if(entity.password_hash == "*",
                  400, "password_hash cannot be placeholder during user creation");

        if (!entity.email.empty())
        {
            return_if(core::has_user_email(ctx, entity.email),
                      409, "email already exists");
        }

        return ok_result;
    }

    OperationResult UserValidator::validate_read_integrity(const RequestContext& ctx, const Model& entity) const
    {
        return ok_result;
    }

    OperationResult UserValidator::validate_update_integrity(const RequestContext& ctx, const Model& old_entity,
                                                             const Model& new_entity) const
    {
        bool logged_user_updates_himself = ctx.token.user_id == old_entity.get_id();

        if (!ctx.token.system)
        {
            return_if(new_entity.password_hash != old_entity.password_hash,
                      400, "password cannot be changed here, use /change-password endpoint instead");
        }

        bool role_different = new_entity.role != old_entity.role;
        return_if(role_different && logged_user_updates_himself,
                  400, "Role cannot be changed. You cannot change your own role.");

        return_if(role_different && ctx.role < mindnet::essential::UserRole::Admin,
                  400, "Role cannot be changed. You must be at least Admin.");
        return_if(role_different && ctx.role == mindnet::essential::UserRole::Admin && old_entity.role == essential::UserRole::SuperAdmin,
          400, "Role cannot be changed. You must be at least SuperAdmin to change role of a SuperAdmin user.");

        return_if(old_entity.status != new_entity.status && ctx.role < mindnet::essential::UserRole::Admin,
                  400, "Status cannot be changed by yourself.")

        return ok_result;
    }

    OperationResult UserValidator::validate_delete_integrity(const RequestContext& ctx, const Model& entity) const
    {
        return ok_result;
    }

    OperationResult UserValidator::validate_list_integrity(const RequestContext& ctx, const string_map& filter) const
    {
        return ok_result;
    }

    string UserValidator::get_model_name() const
    {
        return STRINGIFY(model);
    }
}

#undef Model
#undef MODEL
#undef model
