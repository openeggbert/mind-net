//
// Created by robertvokac on 8/6/25.
//

#include "mindnet/plugins/slipbox/validators/NoteNavigationValidator.h"

#include "mindnet/essential/Global.h"
#include "mindnet/plugins/slipbox/models/NoteNavigation.h"
#include "mindnet/api/Persistence.h"
#include "mindnet/plugins/slipbox/SlipBoxPersistenceMethods.h"

#define Model NoteNavigation
#define MODEL NOTE_NAVIGATION
#define model note_navigation

namespace mindnet::plugins::slipbox::validators
{
    using validators::NoteNavigationValidator;
    using mindnet::api::OperationResult;
    using mindnet::essential::g_configuration;

    OperationResult NoteNavigationValidator::validate_create_authorization(const RequestContext& ctx, const Model& entity) const
    {
        return status_405_unsupported_operation;
    }

    OperationResult NoteNavigationValidator::validate_read_authorization(const RequestContext& ctx, const Model& entity) const
    {
        return ok_result;
    }

    OperationResult NoteNavigationValidator::validate_update_authorization(const RequestContext& ctx, const Model& old_entity,
                                                                 const Model& new_entity) const
    {
        return status_405_unsupported_operation;
    }

    OperationResult NoteNavigationValidator::validate_delete_authorization(const RequestContext& ctx, const Model& entity) const
    {
        return status_405_unsupported_operation;
    }

    OperationResult NoteNavigationValidator::validate_list_authorization(const RequestContext& ctx,
                                                               const string_map& filter) const
    {
        return status_405_unsupported_operation;
    }

    OperationResult NoteNavigationValidator::validate_create_integrity(const RequestContext& ctx, const Model& entity) const
    {
        return status_405_unsupported_operation;
    }

    OperationResult NoteNavigationValidator::validate_read_integrity(const RequestContext& ctx, const Model& entity) const
    {
        return ok_result;
    }

    OperationResult NoteNavigationValidator::validate_update_integrity(const RequestContext& ctx, const Model& old_entity,
                                                             const Model& new_entity) const
    {
        return status_405_unsupported_operation;
    }

    OperationResult NoteNavigationValidator::validate_delete_integrity(const RequestContext& ctx, const Model& entity) const
    {
        return status_405_unsupported_operation;
    }

    OperationResult NoteNavigationValidator::validate_list_integrity(const RequestContext& ctx, const string_map& filter) const
    {
        return status_405_unsupported_operation;
    }

    string NoteNavigationValidator::get_model_name() const
    {
        return STRINGIFY(model);
    }
}

#undef Model
#undef MODEL
#undef model
