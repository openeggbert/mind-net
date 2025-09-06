//
// Created by robertvokac on 8/6/25.
//


#include "mindnet/persistence/impl/sqlite/validators/TeamCrudlValidator.h"

#include "mindnet/Global.h"
#include "mindnet/models/Team.h"
#include "mindnet/persistence/Persistence.h"

namespace mindnet::persistence::impl::sqlite::validators
{
    using impl::sqlite::validators::TeamCrudlValidator;
    operation_result TeamCrudlValidator::can_create(db_& db, http::LoginToken& token, entity_fields& ef) const
    {
        //2. Authorization
        logged_user()

        //
        if (logged_in_user.role < enums::UserRole::EDITOR)
        {
            return operation_result(403, "User does not have permission to create a team.");
        }

        //3. Request
        models::Team new_entity;
        new_entity.from_values(ef);
        err << new_entity << commit;

        string error = new_entity.validate();
        if (!error.empty()) return operation_result(400, error);

        if (new_entity.created_by != logged_in_user.get_id())
            return operation_result(
                400, "created_by must be set to the logged in user.");
        if (new_entity.leader_id != logged_in_user.get_id())
            return operation_result(
                400, "leader_id must be set to the logged in user.");
        return ok_result;
    }

    operation_result TeamCrudlValidator::can_read(db_& db, http::LoginToken& token, int id) const
    {
        return ok_result;
    }

    operation_result TeamCrudlValidator::can_update(db_& db, http::LoginToken& token, entity_fields& ef) const
    {
        //2. Authorization
        logged_user()
        //
        models::Team old_entity;
        old_entity.from_values(ef);
        models::Team new_entity;
        new_entity.from_values(d->read(old_entity.get_id(), models::TEAM_DEFINITION, login_token).first);

        //3. Request
        string error = new_entity.validate();
        if (!error.empty()) return operation_result(400, error);

        if (logged_in_user.role != enums::UserRole::ADMIN && logged_in_user.get_id() != new_entity.leader_id)
            return operation_result(
                403, "Only team leader can update the team.");
        if (old_entity.created_by != new_entity.created_by)
            return
                operation_result(400, "created_by cannot be changed");

        if (old_entity.leader_id != new_entity.leader_id && logged_in_user.role != enums::UserRole::ADMIN)
        {
            return operation_result(400, "leader_id cannot be changed by yourself. Contact admin.");
        }

        return ok_result;
    }

    operation_result TeamCrudlValidator::can_delete(db_& db, http::LoginToken& token, int id) const
    {
        //2. Authorization
        auto logged_in_user_pair = d->find_logged_in_user(login_token);
        if (logged_in_user_pair.second.ko()) return logged_in_user_pair.second;
        auto logged_in_user = logged_in_user_pair.first;
        //
        models::Team team;
        team.from_values(d->read(id, models::TEAM_DEFINITION, login_token).first);


        if (logged_in_user.role != enums::UserRole::ADMIN)
            return operation_result(
                403, "Only admins can delete a team. Contact admin");

        return ok_result;
    }

    operation_result TeamCrudlValidator::can_list(db_& db, string_map& filter,
                                                  http::LoginToken& token) const
    {
        return ok_result;
    }

    string TeamCrudlValidator::get_model_name() const
    {
        return "team";
    }
}
