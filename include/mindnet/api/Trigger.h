//
// Created by robertvokac on 9/7/25.
//
#ifndef TRIGGER_H
#define TRIGGER_H

#include <memory>
#include <string>

#include "mindnet/OperationResult.h"
#include "mindnet/TriggerPhase.h"
#include "mindnet/plugins/core/enums/Crudl.h"
#include <vector>

#include "mindnet/http/QueryParams.h"
#include "mindnet/model/ModelDefinition.h"

namespace mindnet::api
{
    class Trigger
    {
    public:
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

        virtual void run(
            plugins::core::enums::Crudl operation,
            int stack_depth,
            const OperationResult& validation_result,
            const OperationResult& action_result,
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

    };



    typedef std::shared_ptr<Trigger> TriggerPtr;
}

#endif // TRIGGER_H
