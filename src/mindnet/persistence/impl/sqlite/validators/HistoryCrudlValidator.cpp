//
// Created by robertvokac on 8/6/25.
//

#include "mindnet/persistence/impl/sqlite/validators/HistoryCrudlValidator.h"

#include "mindnet/Global.h"
#include "mindnet/models/History.h"
#include "mindnet/persistence/Persistence.h"

#define Model History
#define MODEL HISTORY
#define model history

namespace mindnet::persistence::impl::sqlite::validators
{
    using impl::sqlite::validators::HistoryCrudlValidator;

    operation_result HistoryCrudlValidator::can_create(db_ db, http::LoginToken& token, entity_fields& ef) const
    {
        start_can_create(Model);

        return_if (new_entity.user_id != logged_in_user.get_id(),
            403, "You can only create history for your own user.");

        return ok_result;
    }

    operation_result HistoryCrudlValidator::can_read(db_ db, http::LoginToken& token, int id) const
    {
        start_can_read(Model, MODEL)

        return_if (entity.user_id != logged_in_user.get_id() && !is_admin,
            403, "You can only read history for your own user.");

        //3. Request
        return ok_result;
    }

    operation_result HistoryCrudlValidator::can_update(db_ db, http::LoginToken& token, entity_fields& ef) const
    {
        start_can_update(Model, MODEL)

        return {405, "History cannot be updated."};
    }

    operation_result HistoryCrudlValidator::can_delete(db_ db, http::LoginToken& token, int id) const
    {
        start_can_delete(Model, MODEL)

        return {405, "History cannot be deleted."};

    }

    operation_result HistoryCrudlValidator::can_list(db_ db, http::LoginToken& token, string_map& filter) const
    {
        start_can_list(Model, MODEL)

        if (is_admin) return ok_result;

        mandatory_filter(user_id)

        return_if (filter["user_id"] != std::to_string(logged_in_user.get_id()),
            403, "You can only list history for your own user.");

        return ok_result;
    }

    string HistoryCrudlValidator::get_model_name() const
    {
        return STRING(model);
    }
}

#undef Model
#undef MODEL
#undef model