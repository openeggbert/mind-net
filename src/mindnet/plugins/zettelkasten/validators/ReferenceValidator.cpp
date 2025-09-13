//
// Created by robertvokac on 8/6/25.
//

#include "mindnet/plugins/zettelkasten/validators/ReferenceValidator.h"

#include "mindnet/Global.h"
#include "mindnet/plugins/core/enums/SingleRight.h"
#include "mindnet/plugins/zettelkasten/models/Reference.h"
#include "mindnet/api/Persistence.h"

#define Model Reference
#define MODEL REFERENCE
#define model reference

namespace mindnet::plugins::zettelkasten::validators
{
    using validators::ReferenceValidator;
    using mindnet::OperationResult;

    OperationResult ReferenceValidator::validate_create(const RequestContext& ctx, const Model& entity) const
    {
        assert_editor()
        return ok_result;
    }

    OperationResult ReferenceValidator::validate_read(const RequestContext& ctx, const Model& entity) const
    {
        return ok_result;
    }

    OperationResult ReferenceValidator::validate_update(const RequestContext& ctx, const Model& old_entity,
                                                             const Model& new_entity) const
    {
        assert_editor()
        return ok_result;
    }

    OperationResult ReferenceValidator::validate_delete(const RequestContext& ctx, const Model& entity) const
    {
        assert_editor()
        return ok_result;
    }

    OperationResult ReferenceValidator::validate_list(const RequestContext& ctx, const string_map& filter) const
    {
        return ok_result;
    }

    string ReferenceValidator::get_model_name() const
    {
        return STRINGIFY(model);
    }
}

#undef Model
#undef MODEL
#undef model
