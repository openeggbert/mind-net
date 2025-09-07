//
// Created by robertvokac on 8/6/25.
//
#ifndef IPERSISTENCE_H
#define IPERSISTENCE_H

#include "IRepository.h"
#include "OperationResult.h"
#include "crow/json.h"
#include "mindnet/http/LoginToken.h"

namespace mindnet::persistence::api
{
    class ICrudlValidator;
    using mindnet::models::misc::ModelDefinition;
    using validator = api::ICrudlValidator*;

    class IPersistence
    {
    public:
        IPersistence() = default;
        virtual ~IPersistence() = default; // explicitly make it destructible

        virtual bool has_repository_with_name(const std::string& name);

        virtual std::vector<std::string>& list_repository_names();

        virtual OperationResult can_create(const ModelDefinition& model_definition, http::LoginToken& token, entity_fields& ef) = 0;
        virtual OperationResult   can_read(const ModelDefinition& model_definition, http::LoginToken& token, int id) = 0;
        virtual OperationResult can_update(const ModelDefinition& model_definition, http::LoginToken& token,entity_fields& ef) = 0;
        virtual OperationResult can_delete(const ModelDefinition& model_definition, http::LoginToken& token,int id) = 0;
        virtual OperationResult   can_list(const ModelDefinition& model_definition, http::LoginToken& token,string_map& filter) = 0;
        //
        virtual std::pair<int, OperationResult>         create(const ModelDefinition& def, http::LoginToken& token, entity_fields& fields) = 0;
        virtual std::pair<entity_fields, OperationResult> read(const ModelDefinition& def, http::LoginToken& token, int id) = 0;
        virtual OperationResult                         update(const ModelDefinition& def, http::LoginToken&  token, int id, entity_fields& fields) = 0;
        virtual OperationResult                         remove(ModelDefinition& def, http::LoginToken&  token, int id) = 0;
        virtual std::pair<std::vector<entity_fields>, OperationResult> list(ModelDefinition& def, http::LoginToken& token, http::QueryParams& query_params) = 0;
        //
        virtual std::optional<ModelDefinition> get_model_definition(const string& model_name) = 0;
        //
        virtual entity_fields request_to_entity_fields(crow::json::rvalue& body, enums::Crudl crudl,
                                                                ModelDefinition& def) = 0;

    };
}
#endif // IPERSISTENCE_H
