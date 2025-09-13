//
// Created by robertvokac on 8/6/25.
//

#include "mindnet/persistence/impl/sqlite/validators/ContentCrudlValidator.h"

#include "mindnet/Global.h"
#include "mindnet/plugins/zettelkasten/models/Content.h"
#include "mindnet/persistence/api/Persistence.h"

#define Model Content
#define MODEL CONTENT
#define model content

namespace mindnet::persistence::impl::sqlite::validators
{
    using impl::sqlite::validators::ContentCrudlValidator;
    using api::OperationResult;

    OperationResult ContentCrudlValidator::validate_create(const RequestContext& ctx, const Model& entity) const
    {

        return_if (ctx.role < plugins::core::enums::UserRole::EDITOR,403, "You can not create content.")
        return_if (entity.version != 1,
            404, "version must be 1 during message creation.");

        return ok_result;
    }

    OperationResult ContentCrudlValidator::validate_read(const RequestContext& ctx, const Model& entity) const
    {
        auto note_id = find_note_for_content(ctx, entity.get_id());
        if (!note_id.second.empty()) return {400, note_id.second};
        auto note = find_model(note, note_id.first);
        if (note.second.empty()) return {400, note.second};

        auto note_validator = get_validator("note");
        if (note_validator == nullptr) return {500, "No note validator found"};

        auto note_ = note.first.to_values();

        return note_validator->can_read(ctx.db, ctx.token, note_id.first);
    }

    OperationResult ContentCrudlValidator::validate_update(const RequestContext& ctx, const Model& old_entity, const Model& new_entity) const
    {
        auto result = find_note_for_content(ctx, old_entity.get_id());
        if (!result.second.empty()) return {400, result.second};
        auto one = find_model(note, old_entity.get_id());

        auto ef = one.first.to_values();

        auto note_validator = get_validator("note");
        if (note_validator == nullptr) return {500, "No note validator found"};

        return note_validator->can_update(ctx.db, ctx.token, ef);
    }

    OperationResult ContentCrudlValidator::validate_delete(const RequestContext& ctx, const Model& entity)  const
    {
        auto result = find_note_for_content(ctx, entity.get_id());
        if (!result.second.empty()) return {400, result.second};


        auto note_validator = get_validator("note");
        if (note_validator == nullptr) return {500, "No note validator found"};

        return note_validator->can_delete(ctx.db, ctx.token, result.first);
    }

    OperationResult ContentCrudlValidator::validate_list(const RequestContext& ctx, const string_map& filter) const
    {

        return {405, "Method not allowed"};
    }

    string ContentCrudlValidator::get_model_name() const
    {
        return STRINGIFY(model);
    }
}

#undef Model
#undef MODEL
#undef model