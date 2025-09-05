//
// Created by robertvokac on 9/2/25.
//

#ifndef MIND_NET_CRUDLVALIDATOR_H
#define MIND_NET_CRUDLVALIDATOR_H
#include <memory>

#include "mindnet/persistence/api/CrudlValidatorResult.h"
#include "mindnet/Helper.h"
#include "mindnet/models/misc/BaseModel.h"
#define create_h_methods()\
validator_result can_create(db_ d, entity_fields& ef) const override;\
validator_result can_read(db_ d, int id) const override;\
validator_result can_update(db_ d, entity_fields& ef) const override;\
validator_result can_delete(db_ d, int id) const override;\
validator_result can_list(db_ d, std::map<std::string, std::string>& filter) const override;\
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
        virtual validator_result can_create(db_ d, entity_fields& ef) const = 0;
        virtual validator_result can_read(db_ d, int id) const = 0;
        virtual validator_result can_update(db_ d, entity_fields& ef) const = 0;
        virtual validator_result can_delete(db_ d, int id) const = 0;
        virtual validator_result can_list(db_ d, std::map<std::string, std::string>& filter) const = 0;
        [[nodiscard]] virtual string get_model_name() const = 0;

    };
}


#endif //MIND_NET_CRUDLVALIDATOR_H
