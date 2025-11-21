//
// Created by robertvokac on 9/7/25.
//

#include "mindnet/plugins/slipbox/triggers/AfterCreateTestAttemptTrigger.hpp"
#include "../../../../../../include/mind-net-db-sqlite/mindnet/db/sqlite/queries/GetQuestionIdsSQLiteQuery.hpp"
#include "mindnet/api/AccessTokenContext.hpp"
#include "mindnet/essential/Global.hpp"
#include "mindnet/plugins/slipbox/models/Test.hpp"
#include "mindnet/plugins/slipbox/models/TestAttempt.hpp"
#include "mindnet/util/Utils.hpp"

namespace mindnet::plugins::slipbox::triggers
{
    using_loggers()

    AfterCreateTestAttemptTrigger::AfterCreateTestAttemptTrigger()
        : Trigger(
            "AfterCreateTestAttemptTrigger",
            "Sets the column question_ids after creation of a test_attempt.",
            1000,
            {essential::Crudl::Create},
            api::TriggerPhase::After,
            "test_attempt"
        )
    {
    }


    void AfterCreateTestAttemptTrigger::run_before_or_after(
        mindnet::essential::Crudl operation,
        int stack_depth,
        api::OperationResult& validation_result,
        api::OperationResult& action_result,
        const mindnet::model::ModelDefinition def,
        i64 user_id,
        i64 id,
        entity_fields& fields,
        entity_fields& old_fields,
        const orm::QueryParams query_params)
    {
        if (action_result.ko())
        {
            return;
        }
        models::TestAttempt new_test_attempt;
        new_test_attempt.from_values(fields);

        i64 test_id = new_test_attempt.test_id;

        api::AccessTokenContext token = user_id == 0
                                            ? api::AccessTokenContext(user_id, "system", 403)
                                            : api::AccessTokenContext(user_id, "", 200);
        auto test_result = run_read(models::TEST_DEFINITION, token, test_id, stack_depth);
        if (test_result.second.ko())
        {
            validation_result = {500, test_result.second.error};
            return;
        }
        models::Test test;
        test.from_values(test_result.first);

        nlohmann::json req;
        req["test_attempt"] = new_test_attempt.to_json();
        req["test"] = test.to_json();
        nlohmann::json res;
        std::string question_ids;
        try
        {
            res = call_query(db::sqlite::queries::QUERY_GetQuestionIds, req);

            if (res.contains("error"))
            {
                validation_result = {500, std::string("Loading question ids failed: " + res["error"].get<std::string>())};
                return;
            }

            question_ids = res["question_ids"];

            info << res.dump() << commit;
            info << "Query GetQuestionIds successful" << commit;
        }
        catch (std::exception& e)
        {
            err << "Query GetQuestionIds failed " << e.what() << commit;
            validation_result = {500,std::string("Query FindPreviousAndNextNote failed ")+ e.what()};
            return;
        }
        new_test_attempt.question_ids = question_ids;
        new_test_attempt.started_at = mindnet::util::Utils::current_unix_timestamp_ms();
        auto values = new_test_attempt.to_values();
        auto update_test_attempt = run_update(models::TEST_ATTEMPT_DEFINITION, token, new_test_attempt.get_id(), values, stack_depth);
        if (update_test_attempt.ko())
        {
            validation_result = {500, update_test_attempt.error};
            return;
        }

    }
}
