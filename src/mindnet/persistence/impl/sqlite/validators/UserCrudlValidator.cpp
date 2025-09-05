//
// Created by robertvokac on 8/6/25.
//

#include "mindnet/persistence/impl/sqlite/validators/UserCrudlValidator.h"

#include <regex>
#include "crow/common.h"
#include "mindnet/Configuration.h"
#include "mindnet/Global.h"
#include "mindnet/models/User.h"
#include "mindnet/persistence/Persistence.h"


namespace mindnet::persistence::impl::sqlite::validators
{
    using impl::sqlite::validators::UserCrudlValidator;

    operation_result UserCrudlValidator::can_create(db_ d, entity_fields& ef, http::LoginToken& login_token) const
    {
        //2. Authorization
        models::User new_user;
        new_user.from_values(ef);
        err << new_user << commit;
        models::User logged_in_user = d->find_logged_in_user(login_token).first;

        if (!g_configuration.allow_self_registration && login_token.ko())
        {
            return {401, "You must be logged in to create a user"};
        }
        if (!g_configuration.allow_self_registration && login_token.ok())
        {
            if (logged_in_user.role != enums::UserRole::ADMIN)
            {
                return operation_result(403, "You must be admin to create a user.");
            }
        }

        //3. Request

        string error = new_user.validate();
        if (!error.empty()) return operation_result(400, error);

        {
            enums::UserRole default_role = g_configuration.default_user_role;
            if (logged_in_user.role != enums::UserRole::ADMIN && new_user.role != default_role)
                return operation_result(400, "role" " must be qual to " + enums::user_role_to_string(default_role));
        }

        error.clear();
        http::QueryParams query_params;
        query_params.filters.emplace("name", new_user.username);
        if (!d->list(query_params, models::USER_DEFINITION, login_token).first.empty())
        {
            return operation_result(409, "username already exists");
        }
        error.clear();
        http::QueryParams query_params2;
        query_params2.filters.emplace("email", new_user.email);
        if (!d->list(query_params2, models::USER_DEFINITION, login_token).first.empty())
        {
            return operation_result(409, "email already exists");
        }

        return ok_result;
    }

    operation_result UserCrudlValidator::can_read(db_ d, int id, http::LoginToken& login_token) const
    {
        //2. Authorization
        return ok_result;
        //3. Request
    }

    operation_result UserCrudlValidator::can_update(db_ d, entity_fields& ef, http::LoginToken& login_token) const
    {
        //2. Authorization
        auto logged_in_user_pair = d->find_logged_in_user(login_token);
        if (logged_in_user_pair.second.ko()) return logged_in_user_pair.second;
        auto logged_in_user = logged_in_user_pair.first;
        //
        models::User old_user;
        old_user.from_values(ef);
        models::User new_user;
        new_user.from_values(d->read(old_user.get_id(), models::USER_DEFINITION, login_token).first);

        bool logged_in_user_updates_himself = logged_in_user.get_id() == old_user.get_id();
        if (logged_in_user.role != enums::UserRole::ADMIN)
        {
            if (!logged_in_user_updates_himself)
            {
                return operation_result(403, "You can only update your own user.");
            }
        }
        //3. Request
        string error = new_user.validate();
        if (!error.empty()) return operation_result(400, error);

        if (old_user.username != new_user.username) return operation_result(400, "username cannot be changed");
        if (new_user.password_hash != "*") return operation_result(
            400, "password cannot be changed here, use /changepw endpoint instead");

        bool role_different = new_user.role != old_user.role;
        if (role_different)
        {
            if (logged_in_user_updates_himself) return operation_result(400, "role cannot be changed");
        }
        if (old_user.email != new_user.email) return operation_result(
            400, "email cannot be changed by yourself, contact admin");
        if (old_user.status != new_user.status && logged_in_user.role != enums::UserRole::ADMIN)
        {
            return operation_result(400, "status cannot be changed by yourself");
        }


        return ok_result;
    }

    operation_result UserCrudlValidator::can_delete(db_ d, int id, http::LoginToken& login_token) const
    {
        return operation_result(403, "You are not allowed to delete this user");
    }

    operation_result UserCrudlValidator::can_list(db_ d, std::map<std::string, std::string>& filter,
                                                  http::LoginToken& login_token) const
    {
        return ok_result;
    }

    string UserCrudlValidator::get_model_name() const
    {
        return "user";
    }
}
