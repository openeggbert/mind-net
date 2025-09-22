//
// Created by robertvokac on 8/6/25.
//

#include "mindnet/plugins/slipbox/validators/CollectionItemValidator.h"

#include "mindnet/essential/Global.h"
#include "mindnet/plugins/slipbox/models/CollectionItem.h"
#include "mindnet/api/Persistence.h"

#define Model CollectionItem
#define MODEL COLLECTION_ITEM
#define model collection_item

namespace mindnet::plugins::slipbox::validators
{
    using validators::CollectionItemValidator;
    using mindnet::api::OperationResult;using mindnet::core::g_configuration;


    OperationResult CollectionItemValidator::validate_create_authorization(const RequestContext& ctx, const Model& entity) const
    {
        return ok_result;
    }

    OperationResult CollectionItemValidator::validate_read_authorization(const RequestContext& ctx, const Model& entity) const
    {
        return ok_result;
    }

    OperationResult CollectionItemValidator::validate_update_authorization(const RequestContext& ctx, const Model& old_entity,
                                                                  const Model& new_entity) const
    {
        return ok_result;
    }

    OperationResult CollectionItemValidator::validate_delete_authorization(const RequestContext& ctx, const Model& entity) const
    {
        return ok_result;
    }

    OperationResult CollectionItemValidator::validate_list_authorization(const RequestContext& ctx,
                                                                const string_map& filter) const
    {
        return ok_result;
    }





    OperationResult CollectionItemValidator::validate_create_integrity(const RequestContext& ctx, const Model& entity) const
    {
        assert_editor()
        return ok_result;
    }

    OperationResult CollectionItemValidator::validate_read_integrity(const RequestContext& ctx, const Model& entity) const
    {
        return ok_result;
    }

    OperationResult CollectionItemValidator::validate_update_integrity(const RequestContext& ctx, const Model& old_entity,
                                                                  const Model& new_entity) const
    {
        assert_editor()
        return ok_result;

        return ok_result;
    }

    OperationResult CollectionItemValidator::validate_delete_integrity(const RequestContext& ctx, const Model& entity) const
    {
        assert_editor()
        return ok_result;
    }

    OperationResult CollectionItemValidator::validate_list_integrity(const RequestContext& ctx,
                                                                const string_map& filter) const
    {
        return ok_result;
    }

    string CollectionItemValidator::get_model_name() const
    {
        return STRINGIFY(model);
    }
}

#undef Model
#undef MODEL
#undef model
