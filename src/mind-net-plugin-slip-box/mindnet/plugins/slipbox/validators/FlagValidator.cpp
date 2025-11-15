//
// Created by robertvokac on 8/6/25.
//

#include "mindnet/plugins/slipbox/validators/FlagValidator.hpp"

#include "mindnet/essential/Global.hpp"
#include "mindnet/plugins/core/enums/SingleRight.hpp"
#include "mindnet/plugins/slipbox/models/Flag.hpp"
#include "mindnet/api/Persistence.hpp"

#define Model Flag
#define MODEL FLAG
#define model flag

namespace mindnet::plugins::slipbox::validators
{
    using validators::FlagValidator;
    using mindnet::api::OperationResult;
    using mindnet::essential::g_configuration;

    OperationResult FlagValidator::validate_create_authorization(const RequestContext& ctx, const Model& entity) const
    {
        assert_editor()

        return ok_result;
    }

    OperationResult FlagValidator::validate_read_authorization(const RequestContext& ctx, const Model& entity) const
    {
        return ok_result;
    }

    OperationResult FlagValidator::validate_update_authorization(const RequestContext& ctx, const Model& old_entity,
                                                                 const Model& new_entity) const
    {
        assert_editor()

        return ok_result;
    }

    OperationResult FlagValidator::validate_delete_authorization(const RequestContext& ctx, const Model& entity) const
    {
        assert_editor()

        return ok_result;
    }

    OperationResult FlagValidator::validate_list_authorization(const RequestContext& ctx,
                                                               const string_map& filter) const
    {
        return ok_result;
    }

    OperationResult FlagValidator::validate_create_integrity(const RequestContext& ctx, const Model& entity) const
    {
        return ok_result;
    }

    OperationResult FlagValidator::validate_read_integrity(const RequestContext& ctx, const Model& entity) const
    {
        return ok_result;
    }

    OperationResult FlagValidator::validate_update_integrity(const RequestContext& ctx, const Model& old_entity,
                                                             const Model& new_entity) const
    {
        return ok_result;
    }

    OperationResult FlagValidator::validate_delete_integrity(const RequestContext& ctx, const Model& entity) const
    {
        return ok_result;
    }

    OperationResult FlagValidator::validate_list_integrity(const RequestContext& ctx, const string_map& filter) const
    {
        return ok_result;
    }

    string FlagValidator::get_model_name() const
    {
        return STRINGIFY(model);
    }
}

#undef Model
#undef MODEL
#undef model
