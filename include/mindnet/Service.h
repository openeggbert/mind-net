//
// Created by robertvokac on 9/13/25.
//

#ifndef MIND_NET_SERVICE_H
#define MIND_NET_SERVICE_H
#include "IService.h"

namespace mindnet
{
    using validator = persistence::api::ICrudlValidator*;
    using mindnet::OperationResult;
    using model::ModelDefinition;


    class Service : public IService
    {
    private:
        std::map<std::string, persistence::api::ICrudlValidator*> validators;
        DbPtr db_ptr;

    public:
        Service(const DbPtr& db_ptr);
        ~Service() override;

        bool has_model(const std::string& model_name) override;

        std::vector<std::string>& list_model_names() override;

        std::pair<int, OperationResult> create(const ModelDefinition& def, http::LoginToken& token,
                                               entity_fields& fields) override;
        std::pair<entity_fields, OperationResult>
        read(const ModelDefinition& def, http::LoginToken& token, int id) override;
        OperationResult update(const ModelDefinition& def, http::LoginToken& token, int id,
                               entity_fields& fields) override;
        OperationResult remove(ModelDefinition& def, http::LoginToken& token, int id) override;
        std::pair<std::vector<entity_fields>, OperationResult> list(ModelDefinition& def, http::LoginToken& token,
                                                                    http::QueryParams& query_params) override;


        std::optional<ModelDefinition> get_model_definition(const string& model_name) override;

        entity_fields request_to_entity_fields(
            crow::json::rvalue& body, plugins::core::enums::Crudl crudl, ModelDefinition& def
        ) override;

    private:
        persistence::api::ICrudlValidator* get_validator(const std::string& name);
        OperationResult can_create(const ModelDefinition& model_definition, http::LoginToken& token,
                                   entity_fields& ef) override;
        OperationResult can_read(const ModelDefinition& model_definition, http::LoginToken& token, int id) override;
        OperationResult can_update(const ModelDefinition& model_definition, http::LoginToken& token,
                                   entity_fields& ef) override;
        OperationResult can_delete(const ModelDefinition& model_definition, http::LoginToken& token, int id) override;
        OperationResult can_list(const ModelDefinition& model_definition, http::LoginToken& token,
                                 string_map& filter) override;
        //
    };
}

#endif //MIND_NET_SERVICE_H
