//
// Created by robertvokac on 8/6/25.
//
#ifndef PERSISTENCE_H
#define PERSISTENCE_H

#include "IPersistence.h"
#include "IRepository.h"
#include "PluginRegistry.h"
#include "crow/json.h"
#include "mindnet/http/LoginToken.h"
#include "../model/ModelDefinition.h"
#include "OperationResult.h"

namespace mindnet::api
{
    using mindnet::model::ModelDefinition;
    using mindnet::api::OperationResult;

    class Persistence : public api::IPersistence
    {
    private:
        std::map<std::string, api::IRepository*> repositories;
        std::vector<std::string> repository_names;
        api::IRepository* get_repository(const std::string& name);


    public:
        Persistence(PluginRegistryPtr& get_plugin_registry);
        ~Persistence() override;

        bool has_model_with_name(const std::string& name) override;

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

        entity_fields request_to_entity_fields(crow::json::rvalue& body, plugins::core::enums::Crudl crudl,
                                               ModelDefinition& def) override;
    };
}
#endif // PERSISTENCE_H
