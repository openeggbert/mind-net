//
// Created by robertvokac on 8/6/25.
//

#include "mindnet/persistence/impl/sqlite/validators/ContentCrudlValidator.h"

#include "mindnet/Global.h"
#include "mindnet/models/Content.h"
#include "mindnet/persistence/api/Persistence.h"

#define Model Content
#define MODEL CONTENT
#define model content

namespace mindnet::persistence::impl::sqlite::validators
{
    using impl::sqlite::validators::ContentCrudlValidator;

    OperationResult ContentCrudlValidator::validate_create(const RequestContext& ctx, const Model& entity) const
    {

        return_if (ctx.role < enums::UserRole::EDITOR,403, "You can not create content.")
        return_if (entity.version != 1,
            404, "version must be 1 during message creation.");

        return ok_result;
    }

    OperationResult ContentCrudlValidator::validate_read(const RequestContext& ctx, const Model& entity) const
    {


        return ok_result;
    }

    OperationResult ContentCrudlValidator::validate_update(const RequestContext& ctx, const Model& old_entity, const Model& new_entity) const
    {


        return ok_result;
    }

    OperationResult ContentCrudlValidator::validate_delete(const RequestContext& ctx, const Model& entity)  const
    {


        return ok_result;
    }

    OperationResult ContentCrudlValidator::validate_list(const RequestContext& ctx, const string_map& filter) const
    {

        return ok_result;
    }

    string ContentCrudlValidator::get_model_name() const
    {
        return STRING(model);
    }
}

#undef Model
#undef MODEL
#undef model