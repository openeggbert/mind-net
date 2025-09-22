//
// Created by robertvokac on 8/6/25.
//

#include "mindnet/plugins/slipbox/validators/LinkValidator.h"

#include "../../../../../include/mindnet/core/Global.h"
#include "mindnet/plugins/core/enums/SingleRight.h"
#include "mindnet/plugins/slipbox/models/Link.h"
#include "mindnet/api/Persistence.h"

#define Model Link
#define MODEL LINK
#define model link

namespace mindnet::plugins::slipbox::validators
{
    using validators::LinkValidator;
    using mindnet::api::OperationResult;
    using mindnet::core::g_configuration;
    OperationResult LinkValidator::validate_create_authorization(const RequestContext& ctx, const Model& entity) const
    {
        return ok_result;
    }

    OperationResult LinkValidator::validate_read_authorization(const RequestContext& ctx, const Model& entity) const
    {
        return ok_result;
    }

    OperationResult LinkValidator::validate_update_authorization(const RequestContext& ctx, const Model& old_entity,
                                                                  const Model& new_entity) const
    {
        return ok_result;
    }

    OperationResult LinkValidator::validate_delete_authorization(const RequestContext& ctx, const Model& entity) const
    {
        return ok_result;
    }

    OperationResult LinkValidator::validate_list_authorization(const RequestContext& ctx,
                                                                const string_map& filter) const
    {
        return ok_result;
    }







    OperationResult LinkValidator::validate_create_integrity(const RequestContext& ctx, const Model& entity) const
    {
        assert_editor()
        return ok_result;
    }

    OperationResult LinkValidator::validate_read_integrity(const RequestContext& ctx, const Model& entity) const
    {
        return ok_result;
    }

    OperationResult LinkValidator::validate_update_integrity(const RequestContext& ctx, const Model& old_entity,
                                                        const Model& new_entity) const
    {
        assert_editor()
        return ok_result;
    }

    OperationResult LinkValidator::validate_delete_integrity(const RequestContext& ctx, const Model& entity) const
    {
        assert_editor()
        return ok_result;
    }

    OperationResult LinkValidator::validate_list_integrity(const RequestContext& ctx, const string_map& filter) const
    {
        return ok_result;
    }

    string LinkValidator::get_model_name() const
    {
        return STRINGIFY(model);
    }
}

#undef Model
#undef MODEL
#undef model
