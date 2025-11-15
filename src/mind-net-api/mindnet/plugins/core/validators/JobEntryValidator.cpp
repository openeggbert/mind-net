//
// Created by robertvokac on 8/6/25.
//

#include "mindnet/plugins/core/validators/JobEntryValidator.hpp"

#include "mindnet/essential/Global.hpp"
#include "mindnet/plugins/core/models/JobEntry.hpp"
#include "mindnet/api/Persistence.hpp"

#define Model JobEntry
#define MODEL JOB_ENTRY
#define model job_entry

namespace mindnet::plugins::core::validators
{
    using validators::JobEntryValidator;
    using mindnet::api::OperationResult;
    using mindnet::essential::g_configuration;

    OperationResult JobEntryValidator::validate_create_authorization(const RequestContext& ctx,
                                                                    const Model& entity) const
    {
        return ok_result;
    }

    OperationResult JobEntryValidator::validate_read_authorization(const RequestContext& ctx, const Model& entity) const
    {
        return ok_result;
    }

    OperationResult JobEntryValidator::validate_update_authorization(const RequestContext& ctx, const Model& old_entity,
                                                                    const Model& new_entity) const
    {
        if (ctx.role < mindnet::essential::UserRole::Admin) return status_403_forbidden;

        //admin or super admin
        if (ctx.role < mindnet::essential::UserRole::System)
        {
            if (old_entity.differs_only_in_enabled(new_entity)) { return ok_result; }
            return {400, "You can modify only the column enabled."};
        }
        // system
        return ok_result;
    }

    OperationResult JobEntryValidator::validate_delete_authorization(const RequestContext& ctx,
                                                                    const Model& entity) const
    {
        if (ctx.role < mindnet::essential::UserRole::Admin) return status_403_forbidden;
        return ok_result;
    }

    OperationResult JobEntryValidator::validate_list_authorization(const RequestContext& ctx,
                                                                  const string_map& filter) const
    {
        return ok_result;
    }


    OperationResult JobEntryValidator::validate_create_integrity(const RequestContext& ctx, const Model& entity) const
    {
        return ok_result;
    }

    OperationResult JobEntryValidator::validate_read_integrity(const RequestContext& ctx, const Model& entity) const
    {
        return ok_result;
    }

    OperationResult JobEntryValidator::validate_update_integrity(const RequestContext& ctx, const Model& old_entity,
                                                                const Model& new_entity) const
    {
        return ok_result;
    }

    OperationResult JobEntryValidator::validate_delete_integrity(const RequestContext& ctx, const Model& entity) const
    {
        return ok_result;
    }

    OperationResult JobEntryValidator::validate_list_integrity(const RequestContext& ctx, const string_map& filter) const
    {
        return ok_result;
    }

    string JobEntryValidator::get_model_name() const
    {
        return STRINGIFY(model);
    }
}

#undef Model
#undef MODEL
#undef model
