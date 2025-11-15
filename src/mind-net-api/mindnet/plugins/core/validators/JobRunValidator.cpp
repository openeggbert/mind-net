//
// Created by robertvokac on 8/6/25.
//

#include "mindnet/plugins/core/validators/JobRunValidator.hpp"

#include "mindnet/essential/Global.hpp"
#include "mindnet/plugins/core/models/JobRun.hpp"
#include "mindnet/api/Persistence.hpp"

#define Model JobRun
#define MODEL JOB_RUN
#define model job_run

namespace mindnet::plugins::core::validators
{
    using validators::JobRunValidator;
    using mindnet::api::OperationResult;
    using mindnet::essential::g_configuration;

    OperationResult JobRunValidator::validate_create_authorization(const RequestContext& ctx,
                                                                    const Model& entity) const
    {
        return ok_result;
    }

    OperationResult JobRunValidator::validate_read_authorization(const RequestContext& ctx, const Model& entity) const
    {
        return ok_result;
    }

    OperationResult JobRunValidator::validate_update_authorization(const RequestContext& ctx, const Model& old_entity,
                                                                    const Model& new_entity) const
    {
        return ok_result;
    }

    OperationResult JobRunValidator::validate_delete_authorization(const RequestContext& ctx,
                                                                    const Model& entity) const
    {
        return status_405_unsupported_operation;
    }

    OperationResult JobRunValidator::validate_list_authorization(const RequestContext& ctx,
                                                                  const string_map& filter) const
    {
        return ok_result;
    }


    OperationResult JobRunValidator::validate_create_integrity(const RequestContext& ctx, const Model& entity) const
    {
        return ok_result;
    }

    OperationResult JobRunValidator::validate_read_integrity(const RequestContext& ctx, const Model& entity) const
    {
        return ok_result;
    }

    OperationResult JobRunValidator::validate_update_integrity(const RequestContext& ctx, const Model& old_entity,
                                                                const Model& new_entity) const
    {
        return ok_result;
    }

    OperationResult JobRunValidator::validate_delete_integrity(const RequestContext& ctx, const Model& entity) const
    {
        return status_405_unsupported_operation;
    }

    OperationResult JobRunValidator::validate_list_integrity(const RequestContext& ctx, const string_map& filter) const
    {
        return ok_result;
    }

    string JobRunValidator::get_model_name() const
    {
        return STRINGIFY(model);
    }
}

#undef Model
#undef MODEL
#undef model
