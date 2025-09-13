//
// Created by robertvokac on 8/6/25.
//

#include "../../../../../include/mindnet/plugins/test/validators/SM2StateCrudlValidator.h"

#include "mindnet/Global.h"
#include "mindnet/plugins/core/enums/SingleRight.h"
#include "mindnet/plugins/test/models/SM2State.h"
#include "mindnet/persistence/api/Persistence.h"

#define Model SM2State
#define MODEL SM2_STATE
#define model sm2_state

namespace mindnet::plugins::test::validators
{
    using validators::SM2StateCrudlValidator;
    using mindnet::OperationResult;

    OperationResult SM2StateCrudlValidator::validate_create(const RequestContext& ctx, const Model& entity) const
    {
        return {405, "Unsupported operation."};
    }

    OperationResult SM2StateCrudlValidator::validate_read(const RequestContext& ctx, const Model& entity) const
    {
        return {405, "Unsupported operation."};
    }

    OperationResult SM2StateCrudlValidator::validate_update(const RequestContext& ctx, const Model& old_entity,
                                                            const Model& new_entity) const
    {
        return {405, "Unsupported operation."};
    }

    OperationResult SM2StateCrudlValidator::validate_delete(const RequestContext& ctx, const Model& entity) const
    {
        return {405, "Unsupported operation."};
    }

    OperationResult SM2StateCrudlValidator::validate_list(const RequestContext& ctx, const string_map& filter) const
    {
        return {405, "Unsupported operation."};
    }

    string SM2StateCrudlValidator::get_model_name() const
    {
        return STRINGIFY(model);
    }
}

#undef Model
#undef MODEL
#undef model
