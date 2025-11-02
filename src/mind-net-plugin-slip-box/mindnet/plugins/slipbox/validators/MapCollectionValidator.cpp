//
// Created by robertvokac on 8/6/25.
//

#include "mindnet/plugins/slipbox/validators/MapCollectionValidator.h"

#include "mindnet/essential/Global.h"
#include "mindnet/plugins/slipbox/models/MapCollection.h"
#include "mindnet/api/Persistence.h"
#include "mindnet/plugins/slipbox/SlipBoxPersistenceMethods.h"

#define Model MapCollection
#define MODEL MAP_COLLECTION
#define model map_collection

namespace mindnet::plugins::slipbox::validators
{
    using validators::MapCollectionValidator;
    using mindnet::api::OperationResult;
    using mindnet::essential::g_configuration;
    using_loggers()

    OperationResult MapCollectionValidator::validate_create_authorization(
        const RequestContext& ctx, const Model& entity) const
    {
        return_if(entity.created_by != ctx.token.user_id,
                  400, "Only owner can create map collections")

        return ok_result;
    }

    OperationResult MapCollectionValidator::validate_read_authorization(const RequestContext& ctx,
                                                                        const Model& entity) const
    {
        if (entity.created_by == ctx.token.user_id) return ok_result;
        if (entity.is_public) return ok_result;

        return {403, "You can not read this map collection."};
    }

    OperationResult MapCollectionValidator::validate_update_authorization(
        const RequestContext& ctx, const Model& old_entity,
        const Model& new_entity) const
    {
        if (old_entity.created_by != ctx.token.user_id)
        {
            return {403, "You can not update this map collection."};
        }

        return ok_result;
    }

    OperationResult MapCollectionValidator::validate_delete_authorization(
        const RequestContext& ctx, const Model& entity) const
    {
        if (entity.created_by != ctx.token.user_id)
        {
            return {403, "You can not delete this map collection."};
        }

        return ok_result;
    }

    OperationResult MapCollectionValidator::validate_list_authorization(const RequestContext& ctx,
                                                                        const string_map& filter) const
    {
        orm::QueryParams params;
        params.page_size = 100;
        for (auto& [key, value] : filter)
        {
            params.add_filter(key, value);
        }
        while (true)
        {
            auto collections = ctx.db->list(plugins::slipbox::models::MAP_COLLECTION_DEFINITION, ctx.token, params);
            if (collections.second.ko()) return collections.second;
            if (collections.first.empty()) break;
            for (auto& values : collections.first)
            {
                plugins::slipbox::models::MapCollection collection;
                collection.from_values(values);
                auto check_result = can_read(ctx.db, ctx.token, collection.get_id());
                if (check_result.ko())
                    return {
                        400,
                        std::string("You request list containing map collection with ID ") + std::to_string(
                            collection.get_id()) +
                        ", but you cannot read this map collection. The reason: " + check_result.error
                    };
            }
            params.page_number++;
        }
        return ok_result;
    }


    OperationResult MapCollectionValidator::validate_create_integrity(const RequestContext& ctx,
                                                                      const Model& entity) const
    {
        return_if(entity.created_by != ctx.token.user_id,
          400, "Owner must be the logged in user.")


        return ok_result;
    }

    OperationResult MapCollectionValidator::validate_read_integrity(const RequestContext& ctx,
                                                                    const Model& entity) const
    {
        return ok_result;
    }

    OperationResult MapCollectionValidator::validate_update_integrity(const RequestContext& ctx,
                                                                      const Model& old_entity,
                                                                      const Model& new_entity) const
    {
        return ok_result;
    }

    OperationResult MapCollectionValidator::validate_delete_integrity(const RequestContext& ctx,
                                                                      const Model& entity) const
    {
        return ok_result;
    }

    OperationResult MapCollectionValidator::validate_list_integrity(const RequestContext& ctx,
                                                                    const string_map& filter) const
    {
        return ok_result;
    }

    string MapCollectionValidator::get_model_name() const
    {
        experiment << STRINGIFY(model) << commit;
        return STRINGIFY(model);
    }
}
