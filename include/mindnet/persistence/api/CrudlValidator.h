//
// Created by robertvokac on 9/2/25.
//

#ifndef MIND_NET_CRUDLVALIDATOR_H
#define MIND_NET_CRUDLVALIDATOR_H
#include <memory>

#include "mindnet/Helper.h"
#define create_h_methods()\
string can_create(db_ d, entity_fields& ef) const override;\
string can_read(db_ d, int id) const override;\
string can_update(db_ d, entity_fields& ef) const override;\
string can_delete(db_ d, int id) const override;\
string can_list(db_ d, std::map<std::string, std::string>& filter) const override;\
[[nodiscard]] string get_model_name() const override;

namespace mindnet::persistence
{
    class Persistence;
}

namespace mindnet::persistence::api
{
    using db_ = mindnet::persistence::Persistence*;

    class CrudlValidator
    {
    public:
        CrudlValidator() = default;
        virtual ~CrudlValidator() = default; // explicitly make it destructible
        virtual string can_create(db_ d, entity_fields& ef) const = 0;
        virtual string can_read(db_ d, int id) const = 0;
        virtual string can_update(db_ d, entity_fields& ef) const = 0;
        virtual string can_delete(db_ d, int id) const = 0;
        virtual string can_list(db_ d, std::map<std::string, std::string>& filter) const = 0;
        virtual string get_model_name() const = 0;

    };
}


#endif //MIND_NET_CRUDLVALIDATOR_H
