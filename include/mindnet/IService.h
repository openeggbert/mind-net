//
// Created by robertvokac on 9/13/25.
//

#ifndef MIND_NET_ISERVICE_H
#define MIND_NET_ISERVICE_H
#include "crow/json.h"
#include "http/QueryParams.h"
#include "models/misc/ModelDefinition.h"
#include "persistence/api/ICrudlValidator.h"
#include "mindnet/persistence/api/IPersistence.h"

namespace mindnet{
    using validator = persistence::api::ICrudlValidator*;
    using persistence::api::OperationResult;
    using models::misc::ModelDefinition;
    using persistence::api::DbPtr;

class IService
{
public:
    IService(const DbPtr& db_) {};
    virtual ~IService() = default;


    virtual bool has_model(const std::string& name) = 0;
    virtual std::vector<std::string>& list_model_names() = 0;
    //
    virtual std::pair<int, OperationResult>         create(const ModelDefinition& def, http::LoginToken& token, entity_fields& fields) = 0;
    virtual std::pair<entity_fields, OperationResult> read(const ModelDefinition& def, http::LoginToken& token, int id) = 0;
    virtual OperationResult                         update(const ModelDefinition& def, http::LoginToken&  token, int id, entity_fields& fields) = 0;
    virtual OperationResult                         remove(ModelDefinition& def, http::LoginToken&  token, int id) = 0;
    virtual std::pair<std::vector<entity_fields>, OperationResult> list(ModelDefinition& def, http::LoginToken& token, http::QueryParams& query_params) = 0;
    //
    virtual std::optional<ModelDefinition> get_model_definition(const string& model_name) = 0;
    //
    virtual entity_fields request_to_entity_fields(
        crow::json::rvalue& body,
        enums::Crudl crudl,
        ModelDefinition& def
        ) = 0;

private:
    virtual OperationResult can_create(
        const models::misc::ModelDefinition& model_definition,
        http::LoginToken& token, entity_fields& ef) = 0;
    virtual OperationResult   can_read(const ModelDefinition& model_definition, http::LoginToken& token, int id) = 0;
    virtual OperationResult can_update(const ModelDefinition& model_definition, http::LoginToken& token,entity_fields& ef) = 0;
    virtual OperationResult can_delete(const ModelDefinition& model_definition, http::LoginToken& token,int id) = 0;
    virtual OperationResult   can_list(const ModelDefinition& model_definition, http::LoginToken& token,string_map& filter) = 0;

};
}

#endif //MIND_NET_ISERVICE_H