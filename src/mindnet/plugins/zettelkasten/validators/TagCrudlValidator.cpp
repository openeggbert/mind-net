//
// Created by robertvokac on 8/6/25.
//

#include "../../../../../include/mindnet/plugins/zettelkasten/validators/TagCrudlValidator.h"

#include "mindnet/Global.h"
#include "mindnet/plugins/core/enums/SingleRight.h"
#include "mindnet/plugins/zettelkasten/models/Tag.h"
#include "mindnet/persistence/api/Persistence.h"

#define Model Tag
#define MODEL TAG_H
#define model tag

namespace mindnet::plugins::zettelkasten::validators
{
    using validators::TagCrudlValidator;
    using mindnet::OperationResult;

    OperationResult TagCrudlValidator::validate_create(const RequestContext& ctx, const Model& entity) const
    {

        auto tag_type = find_model(tag_type, entity.tag_type_id);
        if (tag_type.second.empty()) return {400, tag_type.second};

        if(has_right_for_map(ctx, tag_type.first.map_id, plugins::core::enums::SingleRight::WRITE))
        {return ok_result;}
        return {403, "You do not have permission to create a tag for this map."};
    }

    OperationResult TagCrudlValidator::validate_read(const RequestContext& ctx, const Model& entity) const
    {

        auto tag_type = find_model(tag_type, entity.tag_type_id);
        if (tag_type.second.empty()) return {400, tag_type.second};

        auto map = find_model(map, tag_type.first.map_id)
             if (map.second.empty()) return {400, map.second};

        if(has_right_for_map(ctx, tag_type.first.map_id, plugins::core::enums::SingleRight::READ))
        {return ok_result;}
        return {403, "You do not have permission to read this tag."};
    }

    OperationResult TagCrudlValidator::validate_update(const RequestContext& ctx, const Model& old_entity, const Model& new_entity) const
    {
        return {405, "Update of tag_type is forbidden."};
    }

    OperationResult TagCrudlValidator::validate_delete(const RequestContext& ctx, const Model& entity)  const
    {
        auto tag_type = find_model(tag_type, entity.tag_type_id);
        if (tag_type.second.empty()) return {400, tag_type.second};

        if(has_right_for_map(ctx, tag_type.first.map_id, plugins::core::enums::SingleRight::DELETE))
        {return ok_result;}
        return {403, "You do not have permission to delete this tag_type."};
    }

    OperationResult TagCrudlValidator::validate_list(const RequestContext& ctx, const string_map& filter) const
    {
        mandatory_filter(tag_type_id)
        auto tag_type_id = std::stoi(filter.at("tag_type_id"));

        auto tag_type = find_model(tag_type, tag_type_id);
        if (tag_type.second.empty()) return {400, tag_type.second};

        if (!has_right_for_map(ctx, tag_type.first.map_id, plugins::core::enums::SingleRight::READ))
            return {
                403,
                std::string(
                    "You do not have permission to list tag types for map with ID " + std::to_string(tag_type.first.map_id) + ".")
            };

        return ok_result;
    }

    string TagCrudlValidator::get_model_name() const
    {
        return STRINGIFY(model);
    }
}

#undef Model
#undef MODEL
#undef model