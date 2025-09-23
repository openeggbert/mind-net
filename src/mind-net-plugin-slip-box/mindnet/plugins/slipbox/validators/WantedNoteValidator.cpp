//
// Created by robertvokac on 8/6/25.
//

#include "mindnet/plugins/slipbox/validators/WantedNoteValidator.h"

#include "mindnet/essential/Global.h"
#include "mindnet/plugins/slipbox/models/WantedNote.h"
#include "../../../../../../include/mind-net-api/mindnet/api/Persistence.h"
#include "mindnet/plugins/slipbox/SlipBoxPersistenceMethods.h"

#define Model WantedNote
#define MODEL WANTED_NOTE
#define model wanted_note

namespace mindnet::plugins::slipbox::validators
{
    using validators::WantedNoteValidator;
    using mindnet::api::OperationResult;using mindnet::essential::g_configuration;

    OperationResult WantedNoteValidator::validate_create_authorization(const RequestContext& ctx,
                                                                       const Model& entity) const
    {
        return ok_result;
    }

    OperationResult WantedNoteValidator::validate_read_authorization(const RequestContext& ctx,
                                                                  const Model& entity) const
    {
        auto note = slipbox::find_note (ctx, entity.from_note_id);;
        if (!note.second.empty()) return {400, note.second};

        if (!slipbox::has_right_for_map(ctx, note.first.map_id, plugins::core::enums::SingleRight::Read))
        {
            return {403, "You do not have permission to read this wanted note."};
        }
        return ok_result;
    }

    OperationResult WantedNoteValidator::validate_update_authorization(const RequestContext& ctx, const Model& old_entity,
                                                              const Model& new_entity) const
    {
        return status_403_forbidden;
    }

    OperationResult WantedNoteValidator::validate_delete_authorization(const RequestContext& ctx, const Model& entity) const
    {
        return ok_result;
    }


    OperationResult WantedNoteValidator::validate_list_authorization(const RequestContext& ctx,
                                                                     const string_map& filter) const
    {
        mandatory_filter(from_note_id)
        auto note_id = std::stoi(filter.at("from_note_id"));

        auto note = slipbox::find_note (ctx, note_id);;
        if (!note.second.empty()) return {400, note.second};

        if (!slipbox::has_right_for_map(ctx, note.first.map_id, plugins::core::enums::SingleRight::Read))
        {
            return {403, "You do not have permission to list wanted notes for this note."};
        }

        return ok_result;
    }


    OperationResult WantedNoteValidator::validate_create_integrity(const RequestContext& ctx, const Model& entity) const
    {
        return ok_result;
    }


    OperationResult WantedNoteValidator::validate_read_integrity(const RequestContext& ctx, const Model& entity) const
    {
        return ok_result;
    }
    OperationResult WantedNoteValidator::validate_update_integrity(const RequestContext& ctx, const Model& old_entity,
                                                         const Model& new_entity) const
    {
        return status_403_forbidden;
    }
    OperationResult WantedNoteValidator::validate_delete_integrity(const RequestContext& ctx, const Model& entity) const
    {
        return ok_result;
    }

    OperationResult WantedNoteValidator::validate_list_integrity(const RequestContext& ctx,
                                                                 const string_map& filter) const
    {
        return ok_result;
    }

    string WantedNoteValidator::get_model_name() const
    {
        return STRINGIFY(model);
    }
}

#undef Model
#undef MODEL
#undef model
