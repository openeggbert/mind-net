//
// Created by robertvokac on 8/6/25.
//


#include "mindnet/persistence/impl/sqlite/validators/TeamCrudlValidator.h"

#include "mindnet/Global.h"
#include "mindnet/models/Team.h"
#include "mindnet/persistence/Persistence.h"

#define Model Team
#define MODEL TEAM
#define model team

namespace mindnet::persistence::impl::sqlite::validators
{
    using impl::sqlite::validators::TeamCrudlValidator;

    operation_result TeamCrudlValidator::can_create(db_& db, http::LoginToken& token, entity_fields& ef) const
    {
        start_can_create(Model);

        return_if (logged_in_user.role < enums::UserRole::EDITOR,
            403, "User does not have permission to create a team.");

        return_if (new_entity.created_by != logged_in_user.get_id(),
                400, "created_by must be set to the logged in user.")
        return_if (new_entity.leader_id != logged_in_user.get_id(),
                400, "leader_id must be set to the logged in user.")

        return ok_result;
    }

    operation_result TeamCrudlValidator::can_read(db_& db, http::LoginToken& token, int id) const
    {
        return ok_result;
    }

    operation_result TeamCrudlValidator::can_update(db_& db, http::LoginToken& token, entity_fields& ef) const
    {
        start_can_update(Model, MODEL)

        return_if (logged_in_user.role != enums::UserRole::ADMIN && logged_in_user.get_id() != new_entity.leader_id,
            403, "Only team leader can update the team.")
        return_if (old_entity.created_by != new_entity.created_by,
            400, "created_by cannot be changed")

        return_if (old_entity.leader_id != new_entity.leader_id && logged_in_user.role != enums::UserRole::ADMIN,
            400, "leader_id cannot be changed by yourself. Contact admin.")

        return ok_result;
    }

    operation_result TeamCrudlValidator::can_delete(db_& db, http::LoginToken& token, int id) const
    {
        start_can_delete(Model, MODEL)

        if (logged_in_user.role != enums::UserRole::ADMIN)
            return operation_result(
                403, "Only admins can delete a team. Contact admin");

        return ok_result;
    }

    operation_result TeamCrudlValidator::can_list(db_& db, http::LoginToken& token, string_map& filter) const
    {
        return ok_result;
    }

    string TeamCrudlValidator::get_model_name() const
    {
        return "team";
    }
}
