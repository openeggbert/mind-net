//
// Created by robertvokac on 8/6/25.
//
#ifndef IPERSISTENCE_H
#define IPERSISTENCE_H

#include "ICrudlValidator.h"
#include "IRepository.h"
#include "OperationResult.h"
#include "crow/json.h"
#include "mindnet/http/LoginToken.h"



namespace mindnet::persistence::api
{
    using mindnet::models::misc::ModelDefinition;
    using validator = api::ICrudlValidator*;

    class IPersistence
    {
    public:
        IPersistence() = default;
        virtual ~IPersistence() = default; // explicitly make it destructible

        virtual bool has_repository_with_name(const std::string& name);

        virtual std::vector<std::string>& list_repository_names();

        typedef mindnet::persistence::api::OperationResult operation_result;

        virtual operation_result can_create(const ModelDefinition& model_definition, entity_fields& ef, http::LoginToken& login_token) = 0;
        virtual operation_result can_read(const ModelDefinition& model_definition, int id, http::LoginToken& login_token) = 0;
        virtual operation_result can_update(const ModelDefinition& model_definition,entity_fields& ef, http::LoginToken& login_token) = 0;
        virtual operation_result can_delete(const ModelDefinition& model_definition,int id, http::LoginToken& login_token) = 0;
        virtual operation_result can_list(const ModelDefinition& model_definition,string_map& filter, http::LoginToken& login_token) = 0;
        //
        virtual std::pair<int, operation_result> create(const ModelDefinition& def, entity_fields& fields, http::LoginToken& login_token) = 0;
        virtual std::pair<entity_fields, operation_result> read(int id, const ModelDefinition& def, http::LoginToken& login_token) = 0;
        virtual operation_result update(int id, entity_fields& fields, const ModelDefinition& def, http::LoginToken& login_token) = 0;
        virtual operation_result remove(int id, ModelDefinition& def, http::LoginToken& login_token) = 0;
        virtual std::pair<std::vector<entity_fields>, operation_result> list(http::QueryParams& query_params, ModelDefinition& def, http::LoginToken& login_token) = 0;
        //
        virtual std::optional<ModelDefinition> get_model_definition(const string& model_name) = 0;
        //
        virtual entity_fields request_to_entity_fields(crow::json::rvalue& body, enums::Crudl crudl,
                                                                ModelDefinition& def) = 0;

    };
}
#endif // IPERSISTENCE_H
