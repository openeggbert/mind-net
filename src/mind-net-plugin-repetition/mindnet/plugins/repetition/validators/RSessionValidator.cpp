#include "mindnet/plugins/repetition/validators/RSessionValidator.h"

#include "mindnet/essential/Global.h"
#include "mindnet/plugins/core/enums/SingleRight.h"
#include "mindnet/plugins/repetition/models/RSession.h"
#include "mindnet/api/Persistence.h"

#define Model RSession
#define MODEL R_SESSION
#define model r_session

namespace mindnet::plugins::repetition::validators
{
    using validators::RSessionValidator;
    using mindnet::api::OperationResult;
    using mindnet::essential::g_configuration;

    OperationResult RSessionValidator::validate_create_authorization(const RequestContext& ctx,
                                                                     const Model& entity) const
    {
        // User can only create sessions for themselves
        if (ctx.user_id != entity.user_id)
        {
            return {403, "Can only create sessions for yourself"};
        }
        return ok_result;
    }

    OperationResult RSessionValidator::validate_read_authorization(const RequestContext& ctx, const Model& entity) const
    {
        // Users can only read their own sessions
        if (ctx.user_id != entity.user_id)
        {
            return {403, "Can only access your own sessions"};
        }
        return ok_result;
    }

    OperationResult RSessionValidator::validate_update_authorization(const RequestContext& ctx, const Model& old_entity,
                                                                     const Model& new_entity) const
    {
        // Users can only update their own sessions
        if (ctx.user_id != old_entity.user_id || ctx.user_id != new_entity.user_id)
        {
            return {403, "Can only modify your own sessions"};
        }
        return ok_result;
    }

    OperationResult RSessionValidator::validate_delete_authorization(const RequestContext& ctx,
                                                                     const Model& entity) const
    {
        // Users can only delete their own sessions
        if (ctx.user_id != entity.user_id)
        {
            return {403, "Can only delete your own sessions"};
        }
        return ok_result;
    }

    OperationResult RSessionValidator::validate_list_authorization(const RequestContext& ctx,
                                                                   const string_map& filter) const
    {
        // Everyone can list sessions, but results will be filtered by user_id in repository
        return ok_result;
    }

    OperationResult RSessionValidator::validate_create_integrity(const RequestContext& ctx, const Model& entity) const
    {
        // Validate required fields and relationships
        if (entity.algorithm <= 0)
        {
            return {400, "Invalid algorithm specified"};
        }

        if (!entity.notes && !entity.questions)
        {
            return {400, "At least one of notes or questions must be enabled"};
        }

        if (entity.scope <= 0)
        {
            return {400, "Invalid scope specified"};
        }

        // Validate filter combinations
        int filter_count = 0;
        if (entity.filter_under_note > 0) filter_count++;
        if (entity.filter_date_from != 0 || entity.filter_date_to != 0) filter_count++;
        if (entity.filter_tag > 0) filter_count++;
        if (entity.filter_collection > 0) filter_count++;

        if (filter_count > 1)
        {
            return {400, "Only one filter type can be active at a time"};
        }

        return ok_result;
    }

    OperationResult RSessionValidator::validate_read_integrity(const RequestContext& ctx, const Model& entity) const
    {
        return ok_result;
    }

    OperationResult RSessionValidator::validate_update_integrity(const RequestContext& ctx, const Model& old_entity,
                                                                 const Model& new_entity) const
    {
        // Validate the same integrity rules as create
        return validate_create_integrity(ctx, new_entity);
    }

    OperationResult RSessionValidator::validate_delete_integrity(const RequestContext& ctx, const Model& entity) const
    {
        return ok_result;
    }

    OperationResult RSessionValidator::validate_list_integrity(const RequestContext& ctx,
                                                               const string_map& filter) const
    {
        return ok_result;
    }

    string RSessionValidator::get_model_name() const
    {
        return STRINGIFY(model);
    }
}

#undef Model
#undef MODEL
#undef model
