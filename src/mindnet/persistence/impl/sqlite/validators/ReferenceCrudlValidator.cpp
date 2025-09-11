//
// Created by robertvokac on 8/6/25.
//

#include "mindnet/persistence/impl/sqlite/validators/ReferenceCrudlValidator.h"

#include "mindnet/Global.h"
#include "mindnet/enums/SingleRight.h"
#include "mindnet/models/Reference.h"
#include "mindnet/persistence/api/Persistence.h"

#define Model Reference
#define MODEL REFERENCE
#define model reference

namespace mindnet::persistence::impl::sqlite::validators
{
    using impl::sqlite::validators::ReferenceCrudlValidator;

    OperationResult ReferenceCrudlValidator::validate_create(const RequestContext& ctx, const Model& entity) const {
        assert_editor()
        return ok_result;
    }

    OperationResult ReferenceCrudlValidator::validate_read(const RequestContext& ctx, const Model& entity) const
    {
           return ok_result;
    }

    OperationResult ReferenceCrudlValidator::validate_update(const RequestContext& ctx, const Model& old_entity, const Model& new_entity) const
    {
        assert_editor()
          return ok_result;
    }

    OperationResult ReferenceCrudlValidator::validate_delete(const RequestContext& ctx, const Model& entity)  const
    {
        assert_editor()
           return ok_result;
    }

    OperationResult ReferenceCrudlValidator::validate_list(const RequestContext& ctx, const string_map& filter) const
    {
        return ok_result;
    }

    string ReferenceCrudlValidator::get_model_name() const
    {
        return STRING(model);
    }
}

#undef Model
#undef MODEL
#undef model