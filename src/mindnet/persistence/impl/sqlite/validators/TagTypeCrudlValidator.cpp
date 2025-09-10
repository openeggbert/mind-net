//
// Created by robertvokac on 8/6/25.
//

#include "mindnet/persistence/impl/sqlite/validators/TagTypeCrudlValidator.h"

#include "mindnet/Global.h"
#include "mindnet/enums/SingleRight.h"
#include "mindnet/models/TagType.h"
#include "mindnet/persistence/api/Persistence.h"

#define Model TagType
#define MODEL TAG_TYPE
#define model tag_type

namespace mindnet::persistence::impl::sqlite::validators
{
    using impl::sqlite::validators::TagTypeCrudlValidator;

    OperationResult TagTypeCrudlValidator::validate_create(const RequestContext& ctx, const Model& entity) const
    {

        return_if (ctx.role < enums::UserRole::EDITOR,
                  403, "User does not have permission to create a property.")

           if(has_right_for_map(ctx, entity.map_id, enums::SingleRight::WRITE))
           {return ok_result;}
        return {403, "You do not have permission to create a tag type for this map."};

    }

    OperationResult TagTypeCrudlValidator::validate_read(const RequestContext& ctx, const Model& entity) const
    {
        auto map = find_model(map, entity.map_id)
             if (map.second.empty()) return {400, map.second};

        if(has_right_for_map(ctx, entity.map_id, enums::SingleRight::READ))
        {return ok_result;}
        return {403, "You do not have permission to read this tag type."};

    }

    OperationResult TagTypeCrudlValidator::validate_update(const RequestContext& ctx, const Model& old_entity, const Model& new_entity) const
    {
        return {405, "Update of tag_type is forbidden."};
    }

    OperationResult TagTypeCrudlValidator::validate_delete(const RequestContext& ctx, const Model& entity)  const
    {

        if(has_right_for_map(ctx, entity.map_id, enums::SingleRight::DELETE))
        {return ok_result;}
        return {403, "You do not have permission to delete this tag_type."};

    }

    OperationResult TagTypeCrudlValidator::validate_list(const RequestContext& ctx, const string_map& filter) const
    {
        mandatory_filter(map_id)
        auto map_id = std::stoi(filter.at("map_id"));

        if(!has_right_for_map(ctx, map_id, enums::SingleRight::READ))
            return {403, std::string("You do not have permission to list tag types for map with ID " + std::to_string(map_id) + ".")};

        return ok_result;
    }

    string TagTypeCrudlValidator::get_model_name() const
    {
        return STRING(model);
    }
}

#undef Model
#undef MODEL
#undef model