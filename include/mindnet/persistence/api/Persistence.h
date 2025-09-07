//
// Created by robertvokac on 8/6/25.
//
#ifndef DB_H
#define DB_H

#include "IPersistence.h"
#include "IRepository.h"
#include "crow/json.h"
#include "mindnet/http/LoginToken.h"
#include "mindnet/models/misc/ModelDefinition.h"
#include "mindnet/persistence/api/OperationResult.h"

namespace mindnet::persistence::api {
    class CrudlValidator;
}

namespace mindnet::persistence
{
    typedef mindnet::persistence::api::OperationResult operation_result;
    using mindnet::models::misc::ModelDefinition;

    class Persistence : public api::IPersistence
    {
    private:
        std::map<std::string, api::IRepository*> repositories;
        std::vector<std::string> repository_names;
        std::map<std::string, api::ICrudlValidator*> validators;
        api::IRepository* get_repository(const std::string& name);
        api::ICrudlValidator* get_validator(const std::string& name);

    public:
        Persistence();
        ~Persistence();

        bool has_repository_with_name(const std::string& name) override;

        std::vector<std::string>& list_repository_names() override;
        // bool can_user_make_changes(http::LoginToken& login_token, mindnet::persistence::api::OperationResult& value);
        operation_result can_create(const ModelDefinition& model_definition, entity_fields& ef, http::LoginToken& login_token) override;
        operation_result can_read(const ModelDefinition& model_definition, int id, http::LoginToken& login_token) override;
        operation_result can_update(const ModelDefinition& model_definition,entity_fields& ef, http::LoginToken& login_token) override;
        operation_result can_delete(const ModelDefinition& model_definition,int id, http::LoginToken& login_token) override;
        operation_result can_list(const ModelDefinition& model_definition,string_map& filter, http::LoginToken& login_token) override;

        std::pair<int, operation_result> create(const ModelDefinition& def, entity_fields& fields, http::LoginToken& login_token) override;
        std::pair<entity_fields, operation_result> read(int id, const ModelDefinition& def, http::LoginToken& login_token) override;
        operation_result update(int id, entity_fields& fields, const ModelDefinition& def, http::LoginToken& login_token) override;
        operation_result remove(int id, ModelDefinition& def, http::LoginToken& login_token) override;
        std::pair<std::vector<entity_fields>, operation_result> list(http::QueryParams& query_params, ModelDefinition& def, http::LoginToken& login_token) override;

        std::optional<ModelDefinition> get_model_definition(const string& model_name) override;

        entity_fields request_to_entity_fields(crow::json::rvalue& body, enums::Crudl crudl,
                                                                ModelDefinition& def) override;
    };
}
#endif // DB_H
