//
// Created by robertvokac on 8/6/25.
//

#include "../../../../../include/mindnet/plugins/test/validators/ReviewCrudlValidator.h"

#include "mindnet/Global.h"
#include "mindnet/plugins/core/enums/SingleRight.h"
#include "mindnet/plugins/test/models/Review.h"
#include "mindnet/persistence/api/Persistence.h"

#define Model Review
#define MODEL REVIEW
#define model review

namespace mindnet::plugins::test::validators
{
    using validators::ReviewCrudlValidator;
    using mindnet::OperationResult;

    OperationResult ReviewCrudlValidator::validate_create(const RequestContext& ctx, const Model& entity) const
    {
        return {405, "Unsupported operation."};
    }

    OperationResult ReviewCrudlValidator::validate_read(const RequestContext& ctx, const Model& entity) const
    {
        return {405, "Unsupported operation."};
    }

    OperationResult ReviewCrudlValidator::validate_update(const RequestContext& ctx, const Model& old_entity,
                                                          const Model& new_entity) const
    {
        return {405, "Unsupported operation."};
    }

    OperationResult ReviewCrudlValidator::validate_delete(const RequestContext& ctx, const Model& entity) const
    {
        return {405, "Unsupported operation."};
    }

    OperationResult ReviewCrudlValidator::validate_list(const RequestContext& ctx, const string_map& filter) const
    {
        return {405, "Unsupported operation."};
    }

    string ReviewCrudlValidator::get_model_name() const
    {
        return STRINGIFY(model);
    }
}

#undef Model
#undef MODEL
#undef model
