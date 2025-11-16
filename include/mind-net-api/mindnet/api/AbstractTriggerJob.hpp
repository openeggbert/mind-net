//
// Created by robertvokac on 9/7/25.
//
#pragma once

#include <memory>
#include <string>
#include <vector>

#include "OperationResult.hpp"
#include "mindnet/essential/Helper.hpp"

namespace mindnet
{
    namespace orm
    {
        struct QueryParams;
    }

    namespace model
    {
        struct ModelDefinition;
    }

    namespace api
    {
        struct AccessTokenContext;
        class Service;
    }
}

namespace mindnet::api
{
    class AbstractTriggerJob
    {
    public:
        using Service = mindnet::api::Service;

        using CreateFn = std::pair<i64, OperationResult>(Service::*)(const model::ModelDefinition&,
                                                                     api::AccessTokenContext&, entity_fields&, int);
        using ReadFn = std::pair<entity_fields, OperationResult>(Service::*)(
            const model::ModelDefinition&, api::AccessTokenContext&, i64, int);
        using UpdateFn = OperationResult(Service::*)(const model::ModelDefinition&, api::AccessTokenContext&, i64,
                                                     entity_fields&, int);
        using DeleteFn = OperationResult(Service::*)(const model::ModelDefinition&, api::AccessTokenContext&, i64, int);
        using ListFn = std::pair<std::vector<entity_fields>, OperationResult>(Service::*)(
            const model::ModelDefinition&, api::AccessTokenContext&, orm::QueryParams&, int);


        AbstractTriggerJob(
            const std::string& name_,
            const std::string& description_)
            : name(name_),
              description(description_)
        {
        }

        virtual ~AbstractTriggerJob() = default;

        void set_service_ptr(Service* svc) { service_ptr = svc; }
        void set_create_fn(CreateFn fn) { create_fn = fn; }
        void set_read_fn(ReadFn fn) { read_fn = fn; }
        void set_update_fn(UpdateFn fn) { update_fn = fn; }
        void set_delete_fn(DeleteFn fn) { delete_fn = fn; }
        void set_list_fn(ListFn fn) { list_fn = fn; }

        std::pair<i64, OperationResult> run_create(const model::ModelDefinition& def, api::AccessTokenContext& token,
                                                   entity_fields& fields, int depth)
        {
            return (service_ptr->*create_fn)(def, token, fields, depth);
        }

        std::pair<entity_fields, OperationResult> run_read(const model::ModelDefinition& def,
                                                           api::AccessTokenContext& token, i64 id, int depth)
        {
            return (service_ptr->*read_fn)(def, token, id, depth);
        }

        OperationResult run_update(const model::ModelDefinition& def, api::AccessTokenContext& token, i64 id,
                                   entity_fields& fields, int depth)
        {
            return (service_ptr->*update_fn)(def, token, id, fields, depth);
        }

        OperationResult run_delete(const model::ModelDefinition& def, api::AccessTokenContext& token, i64 id, int depth)
        {
            return (service_ptr->*delete_fn)(def, token, id, depth);
        }

        std::pair<std::vector<entity_fields>, OperationResult> run_list(const model::ModelDefinition& def,
                                                                        api::AccessTokenContext& token,
                                                                        orm::QueryParams& query_params, int depth)
        {
            return (service_ptr->*list_fn)(def, token, query_params, depth);
        }

        //
        inline const std::string& get_name() const { return name; }
        inline const std::string& get_description() const { return description; }

    private:
        std::string name;
        std::string description;
        ////
        CreateFn create_fn = nullptr;
        ReadFn read_fn = nullptr;
        UpdateFn update_fn = nullptr;
        DeleteFn delete_fn = nullptr;
        ListFn list_fn = nullptr;
    protected:
        Service* service_ptr = nullptr;
    };

}
