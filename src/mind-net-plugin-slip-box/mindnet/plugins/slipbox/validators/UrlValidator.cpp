//
// Created by robertvokac on 8/6/25.
//

#include "mindnet/plugins/slipbox/validators/UrlValidator.h"

#include "mindnet/essential/Global.h"
#include "mindnet/plugins/core/enums/SingleRight.h"
#include "mindnet/plugins/slipbox/models/Url.h"
#include "mindnet/api/Persistence.h"

#define Model Url
#define MODEL URL
#define model url

namespace mindnet::plugins::slipbox::validators
{
    using validators::UrlValidator;
    using mindnet::api::OperationResult;
    using mindnet::essential::g_configuration;

    OperationResult UrlValidator::validate_create_authorization(const RequestContext& ctx, const Model& entity) const
    {
        assert_editor()
        return ok_result;
    }

    OperationResult UrlValidator::validate_read_authorization(const RequestContext& ctx, const Model& entity) const
    {
        return ok_result;
    }

    OperationResult UrlValidator::validate_update_authorization(const RequestContext& ctx, const Model& old_entity,
                                                                const Model& new_entity) const
    {
        assert_editor()
        return ok_result;
    }

    OperationResult UrlValidator::validate_delete_authorization(const RequestContext& ctx, const Model& entity) const
    {
        assert_editor()
        return ok_result;
    }

    OperationResult UrlValidator::validate_list_authorization(const RequestContext& ctx,
                                                              const string_map& filter) const
    {
        return ok_result;
    }


    OperationResult UrlValidator::validate_create_integrity(const RequestContext& ctx, const Model& entity) const
    {
        return ok_result;
    }

    OperationResult UrlValidator::validate_read_integrity(const RequestContext& ctx, const Model& entity) const
    {
        return ok_result;
    }

    OperationResult UrlValidator::validate_update_integrity(const RequestContext& ctx, const Model& old_entity,
                                                            const Model& new_entity) const
    {
        return ok_result;
    }

    OperationResult UrlValidator::validate_delete_integrity(const RequestContext& ctx, const Model& entity) const
    {
        return ok_result;
    }

    OperationResult UrlValidator::validate_list_integrity(const RequestContext& ctx, const string_map& filter) const
    {
        return ok_result;
    }

    string UrlValidator::get_model_name() const
    {
        return STRINGIFY(model);
    }
}

#undef Model
#undef MODEL
#undef model
