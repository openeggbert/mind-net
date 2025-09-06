//
// Created by robertvokac on 8/6/25.
//

#include "mindnet/persistence/impl/sqlite/validators/UserCrudlValidator.h"

#include <regex>
#include "mindnet/Configuration.h"
#include "mindnet/Global.h"
#include "mindnet/models/User.h"
#include "mindnet/persistence/Persistence.h"

#define Model User
#define MODEL USER
#define model user

namespace mindnet::persistence::impl::sqlite::validators
{
    using impl::sqlite::validators::UserCrudlValidator;

    operation_result UserCrudlValidator::can_create(db_ db, http::LoginToken& token, entity_fields& ef) const
    {
        start_can_create(Model);

        return_if (!g_configuration.allow_self_registration && token.ko(),
            401,"You must be logged in to create a user")

        return_if (!g_configuration.allow_self_registration && token.ok() && logged_in_user.role != enums::UserRole::ADMIN,
            403,"You must be admin to create a user.")

        return_if(role != enums::UserRole::ADMIN && new_entity.role != g_configuration.default_user_role,
            400,"role" " must be qual to " + enums::user_role_to_string(g_configuration.default_user_role))

        return_if (api::has_user_name(db, token, new_entity.username),
            409, "username already exists")

        return_if (api::has_user_email(db, token, new_entity.email),
            409, "email already exists");

        return ok_result;
    }

    operation_result UserCrudlValidator::can_read(db_ db, http::LoginToken& token, int id) const
    {
        return ok_result;
    }

    operation_result UserCrudlValidator::can_update(db_ db, http::LoginToken& token, entity_fields& ef) const
    {
        start_can_update(Model, MODEL)

        bool logged_in_user_updates_himself = logged_in_user.get_id() == old_entity.get_id();

        return_if (logged_in_user.role != enums::UserRole::ADMIN && !logged_in_user_updates_himself,
            403, "You can only update your own user.")

        return_if (new_entity.password_hash != "*",
            400, "password cannot be changed here, use /changepw endpoint instead");

        bool role_different = new_entity.role != old_entity.role;
        return_if (role_different && logged_in_user_updates_himself,
            400, "role cannot be changed");

        return_if (role_different && role != enums::UserRole::ADMIN,
            400, "role cannot be changed");

        return_if (old_entity.status != new_entity.status && logged_in_user.role != enums::UserRole::ADMIN,
        400, "status cannot be changed by yourself")

        return ok_result;
    }

    operation_result UserCrudlValidator::can_delete(db_ db, http::LoginToken& token, int id) const
    {
        return operation_result(403, "You are not allowed to delete this user");
    }

    operation_result UserCrudlValidator::can_list(db_ db, http::LoginToken& token, string_map& filter) const
    {
        return ok_result;
    }

    string UserCrudlValidator::get_model_name() const
    {
        return STRING(model);
    }
}
