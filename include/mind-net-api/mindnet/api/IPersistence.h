//
// Created by robertvokac on 8/6/25.
//
#ifndef IPERSISTENCE_H
#define IPERSISTENCE_H

#include "IRepository.h"
#include "OperationResult.h"
#include "crow/json.h"
#include "LoginToken.h"
#include "mindnet/model/ModelDefinition.h"

namespace mindnet::api
{
    using mindnet::model::ModelDefinition;

    class IPersistence
    {
    public:
        IPersistence() = default;
        virtual ~IPersistence() = default; // explicitly make it destructible

        virtual bool has_model_with_name(const std::string& name) = 0;

        virtual std::vector<std::string>& list_model_names() = 0;
        //
        virtual std::pair<int, OperationResult> create(const ModelDefinition& def, LoginToken& token,
                                                       entity_fields& fields) = 0;
        virtual std::pair<entity_fields, OperationResult> read(const ModelDefinition& def, LoginToken& token,
                                                               int id) = 0;
        virtual OperationResult update(const ModelDefinition& def, LoginToken& token, int id,
                                       entity_fields& fields) = 0;
        virtual OperationResult remove(ModelDefinition& def, LoginToken& token, int id) = 0;
        virtual std::pair<std::vector<entity_fields>, OperationResult> list(
            ModelDefinition& def, LoginToken& token, orm::QueryParams& query_params) = 0;
        //
        virtual std::optional<ModelDefinition> get_model_definition(const string& model_name) = 0;
        //
        virtual entity_fields request_to_entity_fields(crow::json::rvalue& body, mindnet::essential::Crudl crudl,
                                                       ModelDefinition& def) = 0;
    };

    typedef std::shared_ptr<mindnet::api::IPersistence> DbPtr;
}
#endif // IPERSISTENCE_H
