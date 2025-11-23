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


#include "mindnet/plugins/slipbox/validators/TagTypeValidator.hpp"

#include "mindnet/essential/Global.hpp"
#include "mindnet/plugins/core/enums/SingleRight.hpp"
#include "mindnet/plugins/slipbox/models/TagType.hpp"
#include "mindnet/api/Persistence.hpp"
#include "mindnet/plugins/slipbox/SlipBoxPersistenceMethods.hpp"


#define Model TagType
#define MODEL TAG_TYPE
#define model tag_type

namespace mindnet::plugins::slipbox::validators
{
    using validators::TagTypeValidator;
    using mindnet::api::OperationResult;
    using mindnet::essential::g_configuration;

    OperationResult TagTypeValidator::validate_create_authorization(const RequestContext& ctx,
                                                                    const Model& entity) const
    {
        return_if(ctx.role < mindnet::essential::UserRole::Editor,
                  403, "User does not have permission to create a property.")

        if (!slipbox::has_right_for_map(ctx, entity.map_id, plugins::core::enums::SingleRight::Write))
        {
            return {403, "You do not have permission to create a tag type for this map."};
        }
        return ok_result;
    }

    OperationResult TagTypeValidator::validate_read_authorization(const RequestContext& ctx, const Model& entity) const
    {
        auto map = slipbox::find_map(ctx, entity.map_id);
        if (map.second.empty()) return {400, map.second};

        if (!slipbox::has_right_for_map(ctx, entity.map_id, plugins::core::enums::SingleRight::Read))
        {
            return {403, "You do not have permission to read this tag type."};
        }
        return ok_result;
    }

    OperationResult TagTypeValidator::validate_update_authorization(const RequestContext& ctx, const Model& old_entity,
                                                                    const Model& new_entity) const
    {
        return ok_result;
    }

    OperationResult TagTypeValidator::validate_delete_authorization(const RequestContext& ctx,
                                                                    const Model& entity) const
    {
        if (slipbox::has_right_for_map(ctx, entity.map_id, plugins::core::enums::SingleRight::Delete))
        {
            return ok_result;
        }
        return {403, "You do not have permission to delete this tag_type."};
    }

    OperationResult TagTypeValidator::validate_list_authorization(const RequestContext& ctx,
                                                                  const string_map& filter) const
    {
        mandatory_filter(map_id)
        auto map_id = std::stoi(filter.at("map_id"));

        if (!slipbox::has_right_for_map(ctx, map_id, plugins::core::enums::SingleRight::Read))
            return {
                403,
                std::string(
                    "You do not have permission to list tag types for map with ID " + std::to_string(map_id) + ".")
            };

        return ok_result;
    }


    OperationResult TagTypeValidator::validate_create_integrity(const RequestContext& ctx, const Model& entity) const
    {
        return ok_result;
    }

    OperationResult TagTypeValidator::validate_read_integrity(const RequestContext& ctx, const Model& entity) const
    {
        return ok_result;
    }

    OperationResult TagTypeValidator::validate_update_integrity(const RequestContext& ctx, const Model& old_entity,
                                                                const Model& new_entity) const
    {
        return {405, "Update of tag_type is forbidden."};
    }

    OperationResult TagTypeValidator::validate_delete_integrity(const RequestContext& ctx, const Model& entity) const
    {
        return ok_result;
    }

    OperationResult TagTypeValidator::validate_list_integrity(const RequestContext& ctx, const string_map& filter) const
    {
        return ok_result;
    }

    string TagTypeValidator::get_model_name() const
    {
        return STRINGIFY(model);
    }
}

#undef Model
#undef MODEL
#undef model
