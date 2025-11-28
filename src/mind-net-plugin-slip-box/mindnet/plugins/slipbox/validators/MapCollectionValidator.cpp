/*
 * MIT License
 * Copyright (c) 2025 Robert Vokac
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include "mindnet/plugins/slipbox/validators/MapCollectionValidator.hpp"

#include "mindnet/essential/Global.hpp"
#include "mindnet/plugins/slipbox/models/MapCollection.hpp"
#include "mindnet/api/Persistence.hpp"
#include "mindnet/plugins/slipbox/SlipBoxPersistenceMethods.hpp"

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
#undef Model
#undef MODEL
#undef model
