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
#include "mindnet/persistence/Persistence.h"
#define create_h_methods()\
operation_result can_create(db_& d, entity_fields& ef, http::LoginToken& login_token) const override;\
operation_result can_read(db_ d, int id, http::LoginToken& login_token) const override;\
operation_result can_update(db_ d, entity_fields& ef, http::LoginToken& login_token) const override;\
operation_result can_delete(db_ d, int id, http::LoginToken& login_token) const override;\
operation_result can_list(db_ d, std::map<std::string, std::string>& filter, http::LoginToken& login_token) const override;\
[[nodiscard]] string get_model_name() const override;

#define logged_user()\
auto logged_in_user_pair = d->find_logged_in_user(login_token);\
if (logged_in_user_pair.second.ko()) return logged_in_user_pair.second;\
auto logged_in_user = logged_in_user_pair.first;\

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
        virtual operation_result can_create(db_& d, entity_fields& ef, http::LoginToken& login_token) const = 0;
        virtual operation_result can_read(db_ d, int id, http::LoginToken& login_token) const = 0;
        virtual operation_result can_update(db_ d, entity_fields& ef, http::LoginToken& login_token) const = 0;
        virtual operation_result can_delete(db_ d, int id, http::LoginToken& login_token) const = 0;
        virtual operation_result can_list(db_ d, std::map<std::string, std::string>& filter, http::LoginToken& login_token) const = 0;
        [[nodiscard]] virtual string get_model_name() const = 0;


    };

}


#endif //MIND_NET_CRUDLVALIDATOR_H
