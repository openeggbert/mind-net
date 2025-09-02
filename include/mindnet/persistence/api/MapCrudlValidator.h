//
// Created by robertvokac on 9/2/25.
//
#ifndef MAPCRUDLVALIDATOR_H
#define MAPCRUDLVALIDATOR_H


#include <memory>

#include "CrudlValidator.h"
#include "mindnet/Helper.h"

namespace mindnet::persistence
{
    class Persistence;
}

namespace mindnet::persistence::api
{
    using db_ = mindnet::persistence::Persistence*;

    class MapCrudlValidator : public CrudlValidator
    {
    public:
        MapCrudlValidator() = default;
        ~MapCrudlValidator() = default; // explicitly make it destructible
        string can_create(db_ d, entity_fields& ef) const override;
        string can_read(db_ d, int id) const override;
        string can_update(db_ d, entity_fields& ef) const override;
        string can_delete(db_ d, int id) const override;
        string can_list(db_ d, std::map<std::string, std::string>& filter) const override;
        string get_model_name() const override;
    //
    // public:
    //     virtual string validate(enums::Crudl& crudl, db_& d, entity_fields& ef,
    //                             std::map<std::string, std::string>& filter) const
    //     {
    //         switch (crudl)
    //         {
    //         case enums::Crudl::CREATE: return can_create(d, ef);
    //         case enums::Crudl::READ: return can_read(d, ef);
    //         case enums::Crudl::UPDATE: return can_update(d, ef);
    //         case enums::Crudl::DELETE: return can_delete(d, ef);
    //         case enums::Crudl::LIST:
    //             return can_list(d, ef, filter);
    //         default:
    //             return "Unknown crudl: " + enums::crudl_to_string(crudl);
    //         }
    //     };
    //
    //     virtual string validate(enums::Crudl& crudl, db_& d, entity_fields& ef) const
    //     {
    //         std::map<std::string, std::string> empty_map;
    //         return validate(crudl, d, ef, empty_map);
    //     }

    };

}


#endif // MAPCRUDLVALIDATOR_H
