//
// Created by robertvokac on 9/2/25.
//

#ifndef MIND_NET_CRUDLVALIDATOR_H
#define MIND_NET_CRUDLVALIDATOR_H
#include <memory>

#include "mindnet/persistence/api/OperationResult.h"
#include "mindnet/Helper.h"
#include "mindnet/http/LoginToken.h"
#include "mindnet/models/misc/BaseModel.h"
#include "mindnet/persistence/api/PersistenceMethods.h"
#include "Persistence.h"
#include "mindnet/persistence/api/OperationResult.h"

#define create_h_methods()\
operation_result can_create(db_ db, http::LoginToken& token, entity_fields& ef) const override;\
operation_result can_read(db_ db, http::LoginToken& token, int id) const override;\
operation_result can_update(db_ db, http::LoginToken& token, entity_fields& ef) const override;\
operation_result can_delete(db_ db, http::LoginToken& token, int id) const override;\
operation_result can_list(db_ db, http::LoginToken& token, string_map& filter) const override;\
[[nodiscard]] string get_model_name() const override;

#define return_if(condition, status, message) if (condition) return operation_result(status, message);\


#define mandatory_filter(field)\
if (filter.find( STRING(field) ) == filter.end()) return {403, std::string("You can't filter without ") + STRING(field) + "."};

#define find_model(model, id) api:: XPASTE(find_,model) (db, token, id);
#define check_found(f) if (!f.second.empty()) return{400,f.second};



namespace mindnet::persistence
{
    class Persistence;
}

namespace mindnet::persistence::api
{

    typedef mindnet::persistence::api::OperationResult operation_result;

    class CrudlValidator
    {
    public:
        CrudlValidator() = default;
        virtual ~CrudlValidator() = default; // explicitly make it destructible
        virtual operation_result can_create(db_ptr db, http::LoginToken& token, entity_fields& ef) const = 0;
        virtual operation_result can_read(db_ptr db, http::LoginToken& token, int id) const = 0;
        virtual operation_result can_update(db_ptr db, http::LoginToken& token, entity_fields& ef) const = 0;
        virtual operation_result can_delete(db_ptr db, http::LoginToken& token, int id) const = 0;
        virtual operation_result can_list(db_ptr db, http::LoginToken& token, string_map& filter) const = 0;
        [[nodiscard]] virtual string get_model_name() const = 0;

    };

}


#endif //MIND_NET_CRUDLVALIDATOR_H
