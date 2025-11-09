//
// Created by robertvokac on 9/13/25.
//

#include "mindnet/api/Trigger.h"
#include "mindnet/api/Service.h"
//

namespace mindnet::api
{
    void Trigger::run_before_or_after(mindnet::essential::Crudl operation, int stack_depth,
        OperationResult& validation_result, OperationResult& action_result, model::ModelDefinition def, int user_id,
        int id, entity_fields& fields, entity_fields& old_fields, orm::QueryParams query_params)
    {
        throw std::runtime_error("Empty implementation was called: run_before_or_after()");
    }

    std::optional<std::pair<int, OperationResult>> Trigger::run_instead_of_create(int stack_depth,
        OperationResult& validation_result, const model::ModelDefinition& def, int user_id, int id,
        entity_fields& fields)
    {
        throw std::runtime_error("Empty implementation was called: run_instead_of_create()");
    }

    std::optional<std::pair<entity_fields, OperationResult>> Trigger::run_instead_of_read(int stack_depth,
        OperationResult& validation_result, const model::ModelDefinition& def, int user_id, int id)
    {
        throw std::runtime_error("Empty implementation was called: run_instead_of_read()");
    }

    std::optional<OperationResult> Trigger::run_instead_of_update(int stack_depth, OperationResult& validation_result,
        const model::ModelDefinition& def, int user_id, int id, entity_fields& fields, entity_fields& old_fields)
    {
        throw std::runtime_error("Empty implementation was called: run_instead_of_update()");
    }

    std::optional<OperationResult> Trigger::run_instead_of_delete(int stack_depth, OperationResult& validation_result,
        const model::ModelDefinition& def, int user_id, int id)
    {
        throw std::runtime_error("Empty implementation was called: run_instead_of_delete()");
    }

    std::optional<std::pair<std::vector<entity_fields>, OperationResult>> Trigger::run_instead_of_list(int stack_depth, OperationResult& validation_result,
        const model::ModelDefinition& def, int user_id, const orm::QueryParams& query_params)
    {
        throw std::runtime_error("Empty implementation was called: run_instead_of_list()");
    }

    nlohmann::json Trigger::call_query(const std::string& query_name, nlohmann::json& request)
    {
        return service_ptr->call_query(query_name, request);
    }
}
