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
    using api::OperationResult;
    
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

         OperationResult can_create(const ModelDefinition& model_definition, http::LoginToken& token, entity_fields& ef) override;
         OperationResult   can_read(const ModelDefinition& model_definition, http::LoginToken& token, int id) override;
         OperationResult can_update(const ModelDefinition& model_definition, http::LoginToken& token,entity_fields& ef) override;
         OperationResult can_delete(const ModelDefinition& model_definition, http::LoginToken& token,int id) override;
         OperationResult   can_list(const ModelDefinition& model_definition, http::LoginToken& token,string_map& filter) override;
//
        std::pair<int, OperationResult>         create(const ModelDefinition& def, http::LoginToken& token, entity_fields& fields) override;
        std::pair<entity_fields, OperationResult> read(const ModelDefinition& def, http::LoginToken& token, int id) override;
        OperationResult                         update(const ModelDefinition& def, http::LoginToken&  token, int id, entity_fields& fields) override;
        OperationResult                         remove(ModelDefinition& def, http::LoginToken&  token, int id) override;
        std::pair<std::vector<entity_fields>, OperationResult> list(ModelDefinition& def, http::LoginToken& token, http::QueryParams& query_params) override;


        std::optional<ModelDefinition> get_model_definition(const string& model_name) override;

        entity_fields request_to_entity_fields(crow::json::rvalue& body, enums::Crudl crudl,
                                                                ModelDefinition& def) override;
    };
}
#endif // DB_H
