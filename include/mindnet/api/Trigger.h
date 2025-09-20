//
// Created by robertvokac on 9/7/25.
//
#ifndef TRIGGER_H
#define TRIGGER_H

#include <memory>
#include <string>
#include <vector>
#include "mindnet/OperationResult.h"
#include "mindnet/TriggerPhase.h"
#include "mindnet/plugins/core/enums/Crudl.h"
#include "mindnet/http/QueryParams.h"
#include "mindnet/model/ModelDefinition.h"

namespace mindnet {
    namespace http
    {
        struct LoginToken;
    }

    class Service; }

namespace mindnet::api
{
    class Trigger
    {
    public:
    public:
        using Service = mindnet::Service;

        using CreateFn = std::pair<int, OperationResult>(Service::*)(const model::ModelDefinition&, http::LoginToken&, entity_fields&, int);
        using ReadFn   = std::pair<entity_fields, OperationResult>(Service::*)(const model::ModelDefinition&, http::LoginToken&, int, int);
        using UpdateFn = OperationResult(Service::*)(const model::ModelDefinition&, http::LoginToken&, int, entity_fields&, int);
        using DeleteFn = OperationResult(Service::*)(model::ModelDefinition&, http::LoginToken&, int, int);
        using ListFn   = std::pair<std::vector<entity_fields>, OperationResult>(Service::*)(model::ModelDefinition&, http::LoginToken&, http::QueryParams&, int);


          Trigger(
            const std::string& name_,
            const std::string& description_,
            int priority_,
            const std::vector<plugins::core::enums::Crudl>& operations_,
            TriggerPhase phase_,
            const std::string& table_)
            : name(name_),
              description(description_),
              priority(priority_),
              operations(std::move(operations_)),
              phase(phase_),
              table(table_)
        {
        }

        virtual ~Trigger() = default;



        void set_service_ptr(Service* svc) { service_ptr = svc; }
        void set_create_fn(CreateFn fn) { create_fn = fn; }
        void set_read_fn(ReadFn fn) { read_fn = fn; }
        void set_update_fn(UpdateFn fn) { update_fn = fn; }
        void set_delete_fn(DeleteFn fn) { delete_fn = fn; }
        void set_list_fn(ListFn fn) { list_fn = fn; }

        std::pair<int, OperationResult> run_create(const model::ModelDefinition& def, http::LoginToken& token, entity_fields& fields, int depth)
        {
            return (service_ptr->*create_fn)(def, token, fields, depth);
        }

        std::pair<entity_fields, OperationResult> run_read(const model::ModelDefinition& def, http::LoginToken& token, int id, int depth)
        {
            return (service_ptr->*read_fn)(def, token, id, depth);
        }

        OperationResult run_update(const model::ModelDefinition& def, http::LoginToken& token, int id, entity_fields& fields, int depth)
        {
            return (service_ptr->*update_fn)(def, token, id, fields, depth);
        }

        OperationResult run_delete(model::ModelDefinition& def, http::LoginToken& token, int id, int depth)
        {
            return (service_ptr->*delete_fn)(def, token, id, depth);
        }

        std::pair<std::vector<entity_fields>, OperationResult> run_list(model::ModelDefinition& def, http::LoginToken& token, http::QueryParams& query_params, int depth)
        {
            return (service_ptr->*list_fn)(def, token, query_params, depth);
        }

        virtual void run(
            plugins::core::enums::Crudl operation,
            int stack_depth,
            OperationResult& validation_result,
            OperationResult& action_result,
            model::ModelDefinition def,
            int user_id,
            int id,
            entity_fields fields,
            http::QueryParams query_params
        ) = 0;
        //
        inline const std::string& get_name() const { return name; }
        inline const std::string& get_description() const { return description; }
        inline const int get_priority() const { return priority; }
        //
        inline const std::vector<plugins::core::enums::Crudl>& get_operations() const { return operations; }
        inline const TriggerPhase& get_phase() const { return phase; }
        inline const std::string& get_table() const { return table; }

    private:
        //std::string condition;
        std::string name;
        std::string description;
        int priority{};
        //
        std::vector<plugins::core::enums::Crudl> operations;
        TriggerPhase phase = TriggerPhase::Before;
        std::string table;
        ////
        Service* service_ptr;
        CreateFn create_fn = nullptr;
        ReadFn read_fn     = nullptr;
        UpdateFn update_fn = nullptr;
        DeleteFn delete_fn = nullptr;
        ListFn list_fn     = nullptr;

    };



    typedef std::shared_ptr<Trigger> TriggerPtr;
}

#endif // TRIGGER_H
