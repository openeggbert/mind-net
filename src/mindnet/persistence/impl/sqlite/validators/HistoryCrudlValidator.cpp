//
// Created by robertvokac on 8/6/25.
//

#include "mindnet/persistence/impl/sqlite/validators/HistoryCrudlValidator.h"

#include "mindnet/Global.h"
#include "mindnet/models/History.h"
#include "mindnet/persistence/api/Persistence.h"

#define Model History
#define MODEL HISTORY
#define model history

namespace mindnet::persistence::impl::sqlite::validators
{
    using impl::sqlite::validators::HistoryCrudlValidator;

    operation_result HistoryCrudlValidator::validate_create(const RequestContext& ctx, const Model& entity) const
    {


        return_if (entity.user_id != ctx.token.user_id,
            403, "You can only create history for your own user.");

        return ok_result;
    }

    operation_result HistoryCrudlValidator::validate_read(const RequestContext& ctx, const Model& entity) const
    {


        return_if (entity.user_id != ctx.token.user_id && ctx.role != enums::UserRole::ADMIN,
            403, "You can only read history for your own user.");

        //3. Request
        return ok_result;
    }

    operation_result HistoryCrudlValidator::validate_update(const RequestContext& ctx, const Model& old_entity, const Model& new_entity) const
    {


        return {405, "History cannot be updated."};
    }

    operation_result HistoryCrudlValidator::validate_delete(const RequestContext& ctx, const Model& entity)  const
    {


        return {405, "History cannot be deleted."};

    }

    operation_result HistoryCrudlValidator::validate_list(const RequestContext& ctx, const string_map& filter) const
    {


        if (ctx.role != enums::UserRole::ADMIN) return ok_result;

        mandatory_filter(user_id)

        return_if (filter.at("user_id") != std::to_string(ctx.token.user_id),
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