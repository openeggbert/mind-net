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
        auto result = find_note_for_content(ctx, entity.get_id());
        if (!result.second.empty()) return {400, result.second};

        return ctx.db->can_read(models::NOTE_DEFINITION, ctx.token, result.first);
    }

    OperationResult ContentCrudlValidator::validate_update(const RequestContext& ctx, const Model& old_entity, const Model& new_entity) const
    {
        auto result = find_note_for_content(ctx, old_entity.get_id());
        if (!result.second.empty()) return {400, result.second};
        auto one = find_model(note, old_entity.get_id());

        auto ef = one.first.to_values();
        return ctx.db->can_update(models::NOTE_DEFINITION, ctx.token, ef);
    }

    OperationResult ContentCrudlValidator::validate_delete(const RequestContext& ctx, const Model& entity)  const
    {
        auto result = find_note_for_content(ctx, entity.get_id());
        if (!result.second.empty()) return {400, result.second};

        return ctx.db->can_delete(models::NOTE_DEFINITION, ctx.token, result.first);
    }

    OperationResult ContentCrudlValidator::validate_list(const RequestContext& ctx, const string_map& filter) const
    {

        return {405, "Method not allowed"};
    }

    string ContentCrudlValidator::get_model_name() const
    {
        return STRING(model);
    }
}

#undef Model
#undef MODEL
#undef model