//
// Created by robertvokac on 8/6/25.
//

#include "../../../../../include/mindnet/plugins/zettelkasten/validators/LinkCrudlValidator.h"

#include "mindnet/Global.h"
#include "mindnet/plugins/core/enums/SingleRight.h"
#include "mindnet/plugins/zettelkasten/models/Link.h"
#include "mindnet/persistence/api/Persistence.h"

#define Model Link
#define MODEL LINK
#define model link

namespace mindnet::plugins::zettelkasten::validators
{
    using validators::LinkCrudlValidator;
    using mindnet::OperationResult;

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
        return STRINGIFY(model);
    }
}

#undef Model
#undef MODEL
#undef model