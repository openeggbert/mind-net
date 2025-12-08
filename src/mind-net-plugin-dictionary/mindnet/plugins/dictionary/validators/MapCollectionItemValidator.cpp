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

#include "mindnet/plugins/dictionary/validators/MapCollectionItemValidator.hpp"

#include "mindnet/essential/Global.hpp"
#include "mindnet/plugins/dictionary/models/MapCollectionItem.hpp"
#include "mindnet/api/Persistence.hpp"
#include "mindnet/plugins/dictionary/SlipBoxPersistenceMethods.hpp"

#define Model MapCollectionItem
#define MODEL MAP_COLLECTION_ITEM
#define model map_collection_item

namespace mindnet::plugins::dictionary::validators
{
    using validators::MapCollectionItemValidator;
    using mindnet::api::OperationResult;
    using mindnet::essential::g_configuration;
    using_loggers()

    OperationResult MapCollectionItemValidator::validate_create_authorization(
        const RequestContext& ctx, const Model& entity) const
    {
        auto map_collection = slipbox::find_map_collection(ctx, entity.map_collection_id);
        check_found(map_collection);

        return_if(map_collection.first.created_by != ctx.token.user_id,
                  400, "Only collection owner can add maps")

        return ok_result;
    }

    OperationResult MapCollectionItemValidator::validate_read_authorization(
        const RequestContext& ctx, const Model& entity) const
    {
        auto map_collection = slipbox::find_map_collection(ctx, entity.map_collection_id);
        check_found(map_collection);

        if (map_collection.first.created_by == ctx.token.user_id) return ok_result;
        if (map_collection.first.is_public) return ok_result;

        return {403, "You can not read this map collection item."};
    }

    OperationResult MapCollectionItemValidator::validate_update_authorization(
        const RequestContext& ctx, const Model& old_entity,
        const Model& new_entity) const
    {
        auto map_collection = slipbox::find_map_collection(ctx, old_entity.map_collection_id);
        check_found(map_collection);

        return_if(map_collection.first.created_by != ctx.token.user_id,
                  403, "Only collection owner can update map positions")

        return ok_result;
    }

    OperationResult MapCollectionItemValidator::validate_delete_authorization(
        const RequestContext& ctx, const Model& entity) const
    {
        auto map_collection = slipbox::find_map_collection(ctx, entity.map_collection_id);
        check_found(map_collection);

        return_if(map_collection.first.created_by != ctx.token.user_id,
                  403, "Only collection owner can remove maps")

        return ok_result;
    }

    OperationResult MapCollectionItemValidator::validate_list_authorization(const RequestContext& ctx,
                                                                            const string_map& filter) const
    {
        mandatory_filter(map_collection_id)
        auto map_collection_id = std::stoll(filter.at("map_collection_id"));
        auto map_collection = slipbox::find_collection(ctx, map_collection_id);;
        if (!map_collection.second.empty()) return {400, map_collection.second};
        const auto& map_collection_validator = get_validator("map_collection");
        if (map_collection_validator == nullptr)
        {
            return {400, "map_collection_validator could not be loaded."};
        }

        auto can_read_map_collection = map_collection_validator->can_read(ctx.db, ctx.token, map_collection_id);
        if (can_read_map_collection.ko())
            return {
                403,
                std::string(
                    "You do not have permission to list map_collection_items for map_collection with ID " +
                    std::to_string(map_collection_id) + ".")
            };

        return ok_result;
    }

    OperationResult MapCollectionItemValidator::validate_create_integrity(
        const RequestContext& ctx, const Model& entity) const
    {
        auto map_collection = slipbox::find_map_collection(ctx, entity.map_collection_id);
        check_found(map_collection);

        auto map = slipbox::find_map(ctx, entity.map_id);
        check_found(map);

        return ok_result;
    }

    OperationResult MapCollectionItemValidator::validate_read_integrity(const RequestContext& ctx,
                                                                        const Model& entity) const
    {
        return ok_result;
    }

    OperationResult MapCollectionItemValidator::validate_update_integrity(
        const RequestContext& ctx, const Model& old_entity,
        const Model& new_entity) const
    {
        return ok_result;
    }

    OperationResult MapCollectionItemValidator::validate_delete_integrity(
        const RequestContext& ctx, const Model& entity) const
    {
        return ok_result;
    }

    OperationResult MapCollectionItemValidator::validate_list_integrity(const RequestContext& ctx,
                                                                        const string_map& filter) const
    {
        return ok_result;
    }

    string MapCollectionItemValidator::get_model_name() const
    {
        experiment << STRINGIFY(model) << commit;
        return STRINGIFY(model);
    }
}
#undef Model
#undef MODEL
#undef model