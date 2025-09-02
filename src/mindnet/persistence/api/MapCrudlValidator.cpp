//
// Created by robertvokac on 8/6/25.
//


#include "mindnet/persistence/api/MapCrudlValidator.h"

#include "mindnet/Global.h"
#include "mindnet/models/Map.h"
#include "mindnet/persistence/Persistence.h"

namespace mindnet::persistence::api
{
    string MapCrudlValidator::can_create(db_ d, entity_fields& ef) const
    {
        models::Map map;
        map.from_values(ef);
        err << map << commit;
        if (map.name.empty())
        {
            return "Name must not be empty";
        }
        string error;
        http::QueryParams query_params;
        query_params.filters.emplace("name", map.name);
        if (!d->list(query_params, models::MAP_DEFINITION, error).empty())
        {
            return "Map name already exists";
        }

        if (map.description.size() > 50)
        {
            return "Description must not be longer than 50 characters";
        }
        if (map.owner_rights < 0 || map.owner_rights > 7)
        {
            return "owner_rights must be between 0 and 7";
        }
        if (map.team_rights < 0 || map.team_rights > 7)
        {
            return "team_rights must be between 0 and 7";
        }
        if (map.other_rights < 0 || map.other_rights > 7)
        {
            return "other_rights must be between 0 and 7";
        }
        if (map.owner_rights != castint(enums::AccessRight::READ_WRITE_DELETE))
        {
            //todo
            return "Owner rights must be Read+Write+Delete. This is temporary.";
        }
        return "";
    }

    string MapCrudlValidator::can_read(db_ d, int id) const
    {
        return "The validation is not yet implemented.";
    }

    string MapCrudlValidator::can_update(db_ d, entity_fields& ef) const
    {
        return "The validation is not yet implemented.";
    }

    string MapCrudlValidator::can_delete(db_ d, int id) const
    {
        return "The validation is not yet implemented.";
    }

    string MapCrudlValidator::can_list(db_ d, std::map<std::string, std::string>& filter) const
    {
        return "";
    }

    string MapCrudlValidator::get_model_name() const
    {
        return "map";
    }
}
