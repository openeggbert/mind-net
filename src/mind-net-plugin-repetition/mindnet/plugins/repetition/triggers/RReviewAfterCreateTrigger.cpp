//
// Created by robertvokac on 9/7/25.
//

#include "mindnet/plugins/repetition/triggers/RReviewAfterCreateTrigger.h"

#include "mindnet/essential/Global.h"
#include "mindnet/api/AccessTokenContext.h"
#include "mindnet/plugins/core/models/AuthLog.h"
#include "mindnet/plugins/repetition/models/R0State.h"
#include "mindnet/plugins/repetition/models/R18State.h"
#include "mindnet/plugins/repetition/models/R2State.h"
#include "mindnet/plugins/repetition/models/R4State.h"
#include "mindnet/plugins/repetition/models/RReview.h"
#include "mindnet/util/Utils.h"

namespace mindnet::plugins::repetition::triggers
{
    using_loggers()

    RReviewAfterCreateTrigger::RReviewAfterCreateTrigger()
        : Trigger(
            "RReviewAfterCreateTrigger",
            "Inserts or updates a state table after a successful creation of a r_review.",
            1000,
            {essential::Crudl::Create},
            api::TriggerPhase::After,
            "r_review"
        )
    {
    }

    bool is_algorithm_supported(api::OperationResult& validation_result, const models::RReview& r_review, model::ModelDefinition*& model_definition)
    {
        switch (r_review.algorithm)
        {
        case enums::RepetitionAlgorithm::Repetition0: model_definition = &models::R0_STATE_DEFINITION; break;
        // case enums::RepetitionAlgorithm::Repetition2: model_definition = &models::R2_STATE_DEFINITION; break;
        // case enums::RepetitionAlgorithm::Repetition4: model_definition = &models::R4_STATE_DEFINITION; break;
        // case enums::RepetitionAlgorithm::Repetition18: model_definition = &models::R18_STATE_DEFINITION; break;
        default:
            {
                validation_result = {400, "Unsupported algorithm."};
                return false;
            }
        }
        return true;
    }

    //https://super-memory.com/english/ol/beginning.htm#Algorithm
    static int get_next_r0_interval(int previous_repetitions, bool correct) {
        const static std::vector<int> SM0_INTERVALS = {1, 6, 16, 35, 62, 100, 150, 210};

        if (!correct) return SM0_INTERVALS[0];
        int next_index = std::min(previous_repetitions, static_cast<int>(SM0_INTERVALS.size()) - 1);
        return SM0_INTERVALS[next_index];
    }

    void RReviewAfterCreateTrigger::run(
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
        if (action_result.ko())
        {
            return;
        }

        models::RReview r_review;
        r_review.from_values(fields);

        auto token = api::AccessTokenContext(user_id, "", 200);
        model::ModelDefinition* model_definition = nullptr;

        if (!is_algorithm_supported(validation_result, r_review, model_definition)) return;

        int state_record_id{};

        // ============================================================
        // Checking, if the state record already exists.
        // ============================================================

        orm::QueryParams params;
        params.add_filter("user_id", r_review.user_id);
        params.add_filter("note_id", r_review.note_id);

        auto list_result = run_list(*model_definition, token, params, stack_depth);

        if (list_result.second.ko())
        {
            action_result.status = 500;
            action_result.error = list_result.second.error;
            err << "Listing table " << model_definition->get_model_name() << " failed for user_id " << r_review.user_id << " and note_id " << r_review.note_id << list_result.second.error << std::endl;
            return;
        }
        bool state_record_already_exists = !list_result.first.empty();

        // ============================================================
        // Creating new state record, if it does not yet exist.
        // ============================================================

        bool default_state_record_will_be_created = !state_record_already_exists;
        if (default_state_record_will_be_created)
        {
            entity_fields new_state_fields;
            int64_t now = static_cast<int64_t>(util::Utils::current_unix_timestamp_ms());

            switch (r_review.algorithm)
            {
            case enums::RepetitionAlgorithm::Repetition0:
                {
                    models::R0State state;
                    state.user_id = r_review.user_id;
                    state.note_id = r_review.note_id;
                    state.repetitions = 0;
                    state.interval = 1;
                    state.next_review = 0;
                    state.last_review = 0;
                    state.last_quality = 0;

                    new_state_fields = state.to_values();
                    new_state_fields[1] = now;
                    new_state_fields[2] = now;
                }; break;
            case enums::RepetitionAlgorithm::Repetition2: break;
            case enums::RepetitionAlgorithm::Repetition4: break;
            case enums::RepetitionAlgorithm::Repetition18: break;
            }

            auto create_result = run_create(*model_definition, token,  new_state_fields, stack_depth);

            if (create_result.second.ko())
            {
                action_result.status = 500;
                action_result.error = create_result.second.error;
                err << "Creating new state table " << model_definition->get_model_name() << " failed for user_id " << r_review.user_id << " and note_id " << r_review.note_id << create_result.second.error << commit;
                return;
            } else
            {
                state_record_id = create_result.first;
            }
        }
        if (state_record_already_exists) {
            entity_fields entity_fields_ = list_result.first[0];
            int64_t* id_tmp = std::get_if<int64_t>(&entity_fields_[0]);
            if (id_tmp == nullptr)
            {
                err << "id_tmp == nullptr" << commit;
                return;
            }
            state_record_id = *id_tmp;
        }


        // ============================================================
        // Updating state record.
        // ============================================================
        switch (r_review.algorithm)
        {
        case enums::RepetitionAlgorithm::Repetition0:
            {
                bool correct = r_review.grade >= 3;
                auto read_r0_state = run_read(*model_definition, token, state_record_id, stack_depth);

                if (read_r0_state.second.ko())
                {
                    action_result.status = 500;
                    action_result.error = read_r0_state.second.error;
                    err << "Reading r0_state record failed for id " << state_record_id << read_r0_state.second.error << commit;
                    return;
                }
                models::R0State r0_state;
                r0_state.from_values(read_r0_state.first);

                int old_repetitions = r0_state.repetitions;
                r0_state.repetitions = correct ? r0_state.repetitions + 1 : 0;
                // Interval is based on previous repetition count (before increment)
                r0_state.interval = get_next_r0_interval(old_repetitions, correct);
                constexpr static int64_t MILLISECONDS_PER_DAY = 86400L * 1000;
                r0_state.next_review = util::Utils::current_unix_timestamp_ms() + r0_state.interval * MILLISECONDS_PER_DAY;
                r0_state.last_review = r_review.review_date;
                r0_state.last_quality = r_review.grade;
                auto new_values = r0_state.to_values();
                auto r0_state_update = run_update(*model_definition, token, state_record_id, new_values, stack_depth);

                if (r0_state_update.ko())
                {
                    action_result.status = 500;
                    action_result.error = read_r0_state.second.error;
                    err << "Updating r0_state record failed for id " << state_record_id << r0_state_update.error << commit;
                    return;
                }
            }; break;
        case enums::RepetitionAlgorithm::Repetition2: break;
        case enums::RepetitionAlgorithm::Repetition4: break;
        case enums::RepetitionAlgorithm::Repetition18: break;
        }
    }
}
