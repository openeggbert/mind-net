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
#include "mindnet/persistence/Persistence.h"
#define create_h_methods()\
operation_result can_create(db_& db, http::LoginToken& token, entity_fields& ef) const override;\
operation_result can_read(db_& db, http::LoginToken& token, int id) const override;\
operation_result can_update(db_& db, http::LoginToken& token, entity_fields& ef) const override;\
operation_result can_delete(db_& db, http::LoginToken& token, int id) const override;\
operation_result can_list(db_& db, http::LoginToken& token, string_map& filter) const override;\
[[nodiscard]] string get_model_name() const override;

#define logged_user()\
auto logged_in_user_pair = db->find_logged_in_user(token);\
if (logged_in_user_pair.second.ko()) return logged_in_user_pair.second;\
auto logged_in_user = logged_in_user_pair.first;\
auto role = logged_in_user.role;\
auto is_admin = role == enums::UserRole::ADMIN;

#define return_if(condition, status, message) if (condition) return operation_result(status, message);\

#define start_can_create(Model)\
logged_user()\
models::Model new_entity;\
new_entity.from_values(ef);\
\
string error = new_entity.validate();\
if (!error.empty()) return operation_result(400, error);

#define start_can_read(Model, MODEL)\
logged_user()\
models::Model entity;\
entity.from_values(db->read(id, models:: XPASTE(MODEL,_DEFINITION) , token).first);\
err << entity << commit;


#define start_can_update(Model, MODEL)\
logged_user()\
models::Model new_entity;\
new_entity.from_values(ef);\
auto old_entity_values = db->read(new_entity.get_id(), models::XPASTE(MODEL,_DEFINITION), token).first;\
models::Model old_entity;\
old_entity.from_values(old_entity_values);\
string error = new_entity.validate();\
if (!error.empty()) return operation_result(400, error);\
error = validate_readonly(old_entity_values, ef, models::XPASTE(MODEL,_DEFINITION));\
if (!error.empty()) return operation_result(400, error);\

#define start_can_delete(Model, MODEL)\
logged_user()\
models::Model entity;\
entity.from_values(db->read(id, models:: XPASTE(MODEL,_DEFINITION) , token).first);\
err << entity << commit;

#define start_can_list(Model, MODEL)\
logged_user()

#define mandatory_filter(field)\
if (filter.find( STRING(field) ) == filter.end()) return {403, std::string("You can't filter without ") + STRING(field) + "."};

#define find_model(model, id) api:: XPASTE(find_,model) (db, token, id);



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
        virtual operation_result can_create(db_& db, http::LoginToken& token, entity_fields& ef) const = 0;
        virtual operation_result can_read(db_& db, http::LoginToken& token, int id) const = 0;
        virtual operation_result can_update(db_& db, http::LoginToken& token, entity_fields& ef) const = 0;
        virtual operation_result can_delete(db_& db, http::LoginToken& token, int id) const = 0;
        virtual operation_result can_list(db_& db, http::LoginToken& token, string_map& filter) const = 0;
        [[nodiscard]] virtual string get_model_name() const = 0;


    };

}


#endif //MIND_NET_CRUDLVALIDATOR_H
