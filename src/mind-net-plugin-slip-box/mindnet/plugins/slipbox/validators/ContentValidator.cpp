//
// Created by robertvokac on 8/6/25.
//

#include "mindnet/plugins/slipbox/validators/ContentValidator.h"

#include "mindnet/essential/Global.h"
#include "mindnet/plugins/slipbox/models/Content.h"
#include "mindnet/api/Persistence.h"
#include "mindnet/plugins/slipbox/SlipBoxPersistenceMethods.h"

#define Model Content
#define MODEL CONTENT
#define model content

namespace mindnet::plugins::slipbox::validators
{
    using validators::ContentValidator;
    using mindnet::api::OperationResult;
    using mindnet::essential::g_configuration;

    OperationResult ContentValidator::validate_create_authorization(const RequestContext& ctx,
                                                                    const Model& entity) const
    {
        return_if(ctx.role < mindnet::essential::UserRole::Editor, 403, "You can not create content.")

        return ok_result;
    }

    OperationResult ContentValidator::validate_read_authorization(const RequestContext& ctx, const Model& entity) const
    {
        auto note_id = slipbox::find_note_for_content(ctx, entity.get_id());
        if (!note_id.second.empty()) return {400, note_id.second};
        auto note = slipbox::find_note(ctx, note_id.first);;
        if (!note.second.empty()) return {400, note.second};

        auto note_validator = get_validator("note");
        if (note_validator == nullptr) return {500, "No note validator found"};

        auto note_ = note.first.to_values();

        return note_validator->can_read(ctx.db, ctx.token, note_id.first);
    }

    OperationResult ContentValidator::validate_update_authorization(const RequestContext& ctx, const Model& old_entity,
                                                                    const Model& new_entity) const
    {
        auto result = slipbox::find_note_for_content(ctx, old_entity.get_id());
        if (!result.second.empty()) return {400, result.second};
        auto one = slipbox::find_note(ctx, result.first);;

        auto ef = one.first.to_values();

        auto note_validator = get_validator("note");
        if (note_validator == nullptr) return {500, "No note validator found"};

        auto new_values = new_entity.to_values();
        return note_validator->can_update(ctx.db, ctx.token, ef, new_values);
    }

    OperationResult ContentValidator::validate_delete_authorization(const RequestContext& ctx,
                                                                    const Model& entity) const
    {
        auto result = slipbox::find_note_for_content(ctx, entity.get_id());
        if (!result.second.empty()) return {400, result.second};

        auto note_validator = get_validator("note");
        if (note_validator == nullptr) return {500, "No note validator found"};

        return note_validator->can_delete(ctx.db, ctx.token, result.first);
    }

    OperationResult ContentValidator::validate_list_authorization(const RequestContext& ctx,
                                                                  const string_map& filter) const
    {
        return ok_result;
    }


    OperationResult ContentValidator::validate_create_integrity(const RequestContext& ctx, const Model& entity) const
    {
        return_if(entity.version != 1,
                  404, "version must be 1 during message creation.");

        return ok_result;
    }

    OperationResult ContentValidator::validate_read_integrity(const RequestContext& ctx, const Model& entity) const
    {
        return ok_result;
    }

    OperationResult ContentValidator::validate_update_integrity(const RequestContext& ctx, const Model& old_entity,
                                                                const Model& new_entity) const
    {
        return_if(new_entity.version != old_entity.version + 1, 400,
                  "The version must be incremented by exactly 1 during an update.")
        return ok_result;
    }

    OperationResult ContentValidator::validate_delete_integrity(const RequestContext& ctx, const Model& entity) const
    {
        return ok_result;
    }

    OperationResult ContentValidator::validate_list_integrity(const RequestContext& ctx, const string_map& filter) const
    {
        return {405, "Method not allowed"};
    }

    string ContentValidator::get_model_name() const
    {
        return STRINGIFY(model);
    }
}

#undef Model
#undef MODEL
#undef model
