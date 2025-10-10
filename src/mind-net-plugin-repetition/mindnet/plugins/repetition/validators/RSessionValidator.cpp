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
        if (ctx.token.user_id != entity.user_id)
        {
            return {403, "Can only create sessions for yourself"};
        }
        return ok_result;
    }

    OperationResult RSessionValidator::validate_read_authorization(const RequestContext& ctx, const Model& entity) const
    {
        // Users can only read their own sessions
        if (ctx.token.user_id != entity.user_id)
        {
            return {403, "Can only access your own sessions"};
        }
        return ok_result;
    }

    OperationResult RSessionValidator::validate_update_authorization(const RequestContext& ctx, const Model& old_entity,
                                                                     const Model& new_entity) const
    {
        // Users can only update their own sessions
        if (ctx.token.user_id != new_entity.user_id)
        {
            return {403, "Can only modify your own sessions"};
        }
        return ok_result;
    }

    OperationResult RSessionValidator::validate_delete_authorization(const RequestContext& ctx,
                                                                     const Model& entity) const
    {
        // Users can only delete their own sessions
        if (ctx.token.user_id != entity.user_id)
        {
            return {403, "Can only delete your own sessions"};
        }
        return ok_result;
    }

    OperationResult RSessionValidator::validate_list_authorization(const RequestContext& ctx,
                                                                   const string_map& filter) const
    {
        mandatory_filter(user_id)

        // Users can only list their own sessions
        auto it = filter.find("user_id");
        if (it == filter.end() || std::stoi(it->second) != ctx.token.user_id)
        {
            return {403, "Can only list your own sessions"};
        }
        return ok_result;
    }

    OperationResult RSessionValidator::validate_create_integrity(const RequestContext& ctx, const Model& entity) const
    {
        // Validate required fields and relationships
        return_if(entity.selected_items != "{}", 400, "selected_items must be \"{}\" during r_session creation")

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
        return {405, "Unsupported operation."};
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
