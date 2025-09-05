//
// Created by robertvokac on 8/6/25.
//
#ifndef DB_H
#define DB_H

#include "api/CrudlValidator.h"
#include "api/IRepository.h"
#include "crow/json.h"
#include "mindnet/http/LoginToken.h"
#include "mindnet/models/misc/ModelDefinition.h"

namespace mindnet::persistence
{
    using mindnet::models::misc::ModelDefinition;

    using validator = api::CrudlValidator*;
    class Persistence
    {
    private:
        std::map<std::string, api::IRepository*> repositories;
        std::vector<std::string> repository_names;
        std::map<std::string, validator> validators;
        api::IRepository* get_repository(const std::string& name);
        api::CrudlValidator* get_validator(const std::string& name);
        const std::vector<validator> empty_vector;


    public:
        Persistence();
        ~Persistence();

        bool has_repository(const std::string& name);

        std::vector<std::string>& list_repositories();
        // bool can_user_make_changes(http::LoginToken& login_token, mindnet::persistence::api::OperationResult& value);
        operation_result can_create(const ModelDefinition& model_definition, entity_fields& ef, http::LoginToken& login_token);
        operation_result can_read(const ModelDefinition& model_definition, int id, http::LoginToken& login_token);
        operation_result can_update(const ModelDefinition& model_definition,entity_fields& ef, http::LoginToken& login_token);
        operation_result can_delete(const ModelDefinition& model_definition,int id, http::LoginToken& login_token);
        operation_result can_list(const ModelDefinition& model_definition,std::map<std::string, std::string>& filter, http::LoginToken& login_token);

        std::pair<int, operation_result> create(const ModelDefinition& def, entity_fields& fields, http::LoginToken& login_token);
        std::pair<entity_fields, operation_result> read(int id, const ModelDefinition& def, http::LoginToken& login_token);
        operation_result update(int id, entity_fields& fields, const ModelDefinition& def, http::LoginToken& login_token);
        operation_result remove(int id, ModelDefinition& def, http::LoginToken& login_token);
        std::pair<std::vector<entity_fields>, operation_result> list(http::QueryParams& query_params, ModelDefinition& def, http::LoginToken& login_token);

        std::optional<ModelDefinition> get_model_definition(const string& model_name);


        //
        entity_fields request_to_entity_fields(crow::json::rvalue& body, enums::Crudl crudl,
                                                                ModelDefinition& def);
        std::pair<models::User, mindnet::persistence::api::OperationResult> find_logged_in_user(
            http::LoginToken login_token);
    };
}
#endif // DB_H
