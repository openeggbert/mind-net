//
// Created by robertvokac on 8/6/25.
//

#include "mindnet/persistence/impl/sqlite/validators/LinkCrudlValidator.h"

#include "mindnet/Global.h"
#include "mindnet/enums/SingleRight.h"
#include "mindnet/models/Link.h"
#include "mindnet/persistence/api/Persistence.h"

#define Model Link
#define MODEL LINK
#define model link

namespace mindnet::persistence::impl::sqlite::validators
{
    using impl::sqlite::validators::LinkCrudlValidator;

    OperationResult LinkCrudlValidator::validate_create(const RequestContext& ctx, const Model& entity) const {
        assert_editor()
           return ok_result;
    }

    OperationResult LinkCrudlValidator::validate_read(const RequestContext& ctx, const Model& entity) const
    {
           return ok_result;
    }

    OperationResult LinkCrudlValidator::validate_update(const RequestContext& ctx, const Model& old_entity, const Model& new_entity) const
    {
        assert_editor()
           return ok_result;
    }

    OperationResult LinkCrudlValidator::validate_delete(const RequestContext& ctx, const Model& entity)  const
    {
        assert_editor()
           return ok_result;
    }

    OperationResult LinkCrudlValidator::validate_list(const RequestContext& ctx, const string_map& filter) const
    {

        return ok_result;
    }

    string LinkCrudlValidator::get_model_name() const
    {
        return STRING(model);
    }
}

#undef Model
#undef MODEL
#undef model