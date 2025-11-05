//
// Created by robertvokac on 9/7/25.
//

#include "mindnet/plugins/core/triggers/HistoryCommonTrigger.h"

#include "mindnet/essential/Global.h"
#include "mindnet/api/AccessTokenContext.h"
#include "mindnet/plugins/core/models/History.h"
#include "mindnet/util/Utils.h"

namespace mindnet::plugins::core::triggers
{
    using_loggers()

    HistoryCommonTrigger::HistoryCommonTrigger()
        : Trigger(
            "HistoryCommonTrigger",
            "Logs operations and saves them to the history table.",
            1000,
            {},
            api::TriggerPhase::After,
            "*"
        )
    {
    }

    void HistoryCommonTrigger::run(
        mindnet::essential::Crudl operation,
        int stack_depth,
        api::OperationResult& validation_result,
        api::OperationResult& action_result,
        const mindnet::model::ModelDefinition def,
        int user_id,
        int id,
        entity_fields& fields,
        entity_fields& old_fields,
        const orm::QueryParams query_params)
    {
        if (def.get_model_name() == "history") return;
        if (def.get_model_name() == "api_log") return;
        if (def.get_model_name() == "auth_log") return;
        if (def.get_model_name() == "super_admin_log") return;

        if (action_result.ko()) return;

        experiment << "Trigger: " << commit;
        experiment << "Hello trigger " << commit;
        experiment << "get_name " << get_name() << commit;
        experiment << "get_priority " << get_priority() << commit;
        for (auto& o : get_operations())
        {
            experiment << "get_operations " << crudl_to_string(o) << commit;
        }
        experiment << "get_description " << get_description() << commit;
        experiment << "get_phase " << trigger_phase_to_string(get_phase()) << commit;
        experiment << "get_table " << get_table() << commit;

        experiment << "Arguments: " << commit;
        experiment << "operation " << crudl_to_string(operation) << commit;
        experiment << "validation_result.status " << validation_result.status << commit;
        experiment << "validation_result.error " << validation_result.error << commit;
        experiment << "action_result.status " << action_result.status << commit;
        experiment << "action_result.error " << action_result.error << commit;
        experiment << "def.get_model_name() " << def.get_model_name() << commit;
        experiment << "user_id " << user_id << commit;
        experiment << "id " << id << commit;
        experiment << "fields.size() " << fields.size() << commit;
        experiment << "query_params.fields.size() " << query_params.fields.size() << commit;
        api::AccessTokenContext token = user_id == 0
                                            ? api::AccessTokenContext(user_id, "system", 403)
                                            : api::AccessTokenContext(user_id, "", 200);

        models::History history;

        history.user_id = user_id;
        history.table_name = def.get_model_name();
        history.record_id = id;
        history.operation = operation;
        if (operation == mindnet::essential::Crudl::Create || operation == mindnet::essential::Crudl::Update)
        {
            history.data_json = model_to_json(fields, def).dump();
        }
        else
        {
            history.data_json = "{}";
        }
        history.reason = "";
        auto f = history.to_values();
        int64_t now = static_cast<int64_t>(util::Utils::current_unix_timestamp_ms());
        f[1] = now;
        f[2] = now;

        auto create_result = run_create(models::HISTORY_DEFINITION, token, f, stack_depth);

        if (create_result.second.ko())
        {
            action_result.status = 500;
            action_result.error = create_result.second.error;
        }
        // auto result = run_read(slipbox::models::NOTE_DEFINITION, token, 1, stack_depth);
        //         if (result.second.ko()) experiment << result.second.error << commit;
        //         if (result.second.ok())
        //         {
        //             slipbox::models::Note note;
        //             note.from_values(result.first);
        //             std::cout << note << std::endl;
        //         }
        //     }
    }
}
