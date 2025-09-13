//
// Created by robertvokac on 8/6/25.
//

#include "../../../../../include/mindnet/plugins/zettelkasten/validators/CollectionItemValidator.h"

#include "mindnet/Global.h"
#include "mindnet/plugins/zettelkasten/models/CollectionItem.h"
#include "mindnet/persistence/api/Persistence.h"

#define Model CollectionItem
#define MODEL COLLECTION_ITEM
#define model collection_item

namespace mindnet::plugins::zettelkasten::validators
{
    using validators::CollectionItemValidator;
    using mindnet::OperationResult;

    OperationResult CollectionItemValidator::validate_create(const RequestContext& ctx, const Model& entity) const
    {
        assert_editor()
        return ok_result;
    }

    OperationResult CollectionItemValidator::validate_read(const RequestContext& ctx, const Model& entity) const
    {
        return ok_result;
    }

    OperationResult CollectionItemValidator::validate_update(const RequestContext& ctx, const Model& old_entity,
                                                                  const Model& new_entity) const
    {
        assert_editor()
        return ok_result;

        return ok_result;
    }

    OperationResult CollectionItemValidator::validate_delete(const RequestContext& ctx, const Model& entity) const
    {
        assert_editor()
        return ok_result;
    }

    OperationResult CollectionItemValidator::validate_list(const RequestContext& ctx,
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
