//
// Created by robertvokac on 8/6/25.
//


#include "mindnet/persistence/impl/sqlite/validators/SM2StateCrudlValidator.h"

#include "mindnet/Global.h"
#include "mindnet/persistence/Persistence.h"

namespace mindnet::persistence::impl::sqlite::validators
{
    using impl::sqlite::validators::SM2StateCrudlValidator;
    operation_result SM2StateCrudlValidator::can_create(db_& db, entity_fields& ef) const
    {
        // models::Map map;
        // map.from_values(ef);
        // err << map << commit;
        // if (map.name.empty())
        // {
        //     return "Name must not be empty";
        // }
        // string error;
        // http::QueryParams query_params;
        // query_params.filters.emplace("name", map.name);
        // if (!d->list(query_params, models::MAP_DEFINITION, error).empty())
        // {
        //     return "Map name already exists";
        // }
        //
        // if (map.description.size() > 50)
        // {
        //     return "Description must not be longer than 50 characters";
        // }
        // if (map.owner_id != 1)
        // {
        //     return "Only owner can create maps";
        // }
        // if (map.team_id != 0)
        // {
        //     return "Team maps are not supported yet";
        // }
        // if (map.owner_rights < 0 || map.owner_rights > 7)
        // {
        //     return "owner_rights must be between 0 and 7";
        // }
        // if (map.team_rights < 0 || map.team_rights > 7)
        // {
        //     return "team_rights must be between 0 and 7";
        // }
        // if (map.other_rights < 0 || map.other_rights > 7)
        // {
        //     return "other_rights must be between 0 and 7";
        // }
        // if (map.owner_rights != castint(enums::AccessRight::READ_WRITE_DELETE))
        // {
        //     //todo
        //     return "Owner rights must be Read+Write+Delete. This is temporary.";
        // }
        // if (map.team_rights != castint(enums::AccessRight::NONE))
        // {
        //     return "Team rights must be NONE. This is temporary.";
        // }
        // if (map.other_rights != castint(enums::AccessRight::NONE))
        // {
        //     return "Other rights must be NONE. This is temporary.";
        // }
        return "";
    }

    operation_result SM2StateCrudlValidator::can_read(db_& db, int id) const
    {
        return "The validation is not yet implemented.";
    }

    operation_result SM2StateCrudlValidator::can_update(db_& db, entity_fields& ef) const
    {
        return "The validation is not yet implemented.";
    }

    operation_result SM2StateCrudlValidator::can_delete(db_& db, int id) const
    {
        return "The validation is not yet implemented.";
    }

    operation_result SM2StateCrudlValidator::can_list(db_& db, string_map& filter) const
    {
        return "";
    }

    string SM2StateCrudlValidator::get_model_name() const
    {
        return "todo";
    }
}
