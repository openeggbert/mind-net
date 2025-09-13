//
// Created by robertvokac on 8/6/25.
//

#include "mindnet/persistence/impl/sqlite/validators/CollectionCrudlValidator.h"

#include "mindnet/Global.h"
#include "mindnet/plugins/core/enums/SingleRight.h"
#include "mindnet/plugins/zettelkasten/models/Collection.h"
#include "mindnet/persistence/api/Persistence.h"

#define Model Collection
#define MODEL COLLECTION
#define model collection

namespace mindnet::persistence::impl::sqlite::validators
{
    using impl::sqlite::validators::CollectionCrudlValidator;
    using api::OperationResult;

    OperationResult CollectionCrudlValidator::validate_create(const RequestContext& ctx, const Model& entity) const
    {
        assert_editor()
        return ok_result;
    }

    OperationResult CollectionCrudlValidator::validate_read(const RequestContext& ctx, const Model& entity) const
    {
        return ok_result;
    }

    OperationResult CollectionCrudlValidator::validate_update(const RequestContext& ctx, const Model& old_entity, const Model& new_entity) const
    {
        assert_editor()
        return ok_result;
    }

    OperationResult CollectionCrudlValidator::validate_delete(const RequestContext& ctx, const Model& entity)  const
    {
        assert_editor()

        return ok_result;
    }

    OperationResult CollectionCrudlValidator::validate_list(const RequestContext& ctx, const string_map& filter) const
    {
        return ok_result;
    }

    string CollectionCrudlValidator::get_model_name() const
    {
        return STRINGIFY(model);
    }
}

#undef Model
#undef MODEL
#undef model