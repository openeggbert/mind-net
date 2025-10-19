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

    constexpr static int64_t MILLISECONDS_PER_DAY = 86400L * 1000;

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

    bool is_algorithm_supported(api::OperationResult& validation_result, const models::RReview& r_review,
                                model::ModelDefinition*& model_definition)
    {
        switch (r_review.algorithm)
        {
        case enums::RepetitionAlgorithm::Repetition0: model_definition = &models::R0_STATE_DEFINITION;
            break;
        case enums::RepetitionAlgorithm::Repetition2: model_definition = &models::R2_STATE_DEFINITION;
            break;
        case enums::RepetitionAlgorithm::Repetition4: model_definition = &models::R4_STATE_DEFINITION; break;
        case enums::RepetitionAlgorithm::Repetition18: model_definition = &models::R18_STATE_DEFINITION; break;
        default:
            {
                validation_result = {400, "Unsupported algorithm."};
                return false;
            }
        }
        return true;
    }

    //https://super-memory.com/english/ol/beginning.htm#Algorithm
    static int get_next_r0_interval(int previous_repetitions, bool correct)
    {
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
            err << "Listing table " << model_definition->get_model_name() << " failed for user_id " << r_review.user_id
                << " and note_id " << r_review.note_id << list_result.second.error << std::endl;
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
                };
                break;
            case enums::RepetitionAlgorithm::Repetition2:
                {
                    models::R2State state;
                    state.user_id = r_review.user_id;
                    state.note_id = r_review.note_id;

                    state.repetitions = 0;
                    state.interval = 1;
                    state.ef_times_100 = 250;

                    state.next_review = 0;
                    state.last_review = 0;
                    state.last_quality = 0;

                    new_state_fields = state.to_values();
                };
                break;
            case enums::RepetitionAlgorithm::Repetition4:
                {
                    models::R4State state;
                    state.user_id = r_review.user_id;
                    state.note_id = r_review.note_id;

                    state.repetitions = 0;
                    state.interval = 1;
                    state.ef_times_100 = 250;
                    state.correction_factor_times_100 = 100;

                    state.next_review = 0;
                    state.last_review = 0;
                    state.last_quality = 0;

                    new_state_fields = state.to_values();
                };
                break;
            case enums::RepetitionAlgorithm::Repetition18:
                {
                    models::R18State state;
                    state.user_id = r_review.user_id;
                    state.note_id = r_review.note_id;

                    state.stability_times_100 = 100;
                    state.last_interval_times_100 = 0;
                    state.repetitions = 0;
                    state.lapses = 0;

                    state.next_review = 0;
                    state.last_review = 0;
                    state.last_quality = 0;

                    new_state_fields = state.to_values();
                };
                break;
            }

            new_state_fields[1] = now;
            new_state_fields[2] = now;

            auto create_result = run_create(*model_definition, token, new_state_fields, stack_depth);

            if (create_result.second.ko())
            {
                action_result.status = 500;
                action_result.error = create_result.second.error;
                err << "Creating new state table " << model_definition->get_model_name() << " failed for user_id " <<
                    r_review.user_id << " and note_id " << r_review.note_id << create_result.second.error << commit;
                return;
            }
            else
            {
                state_record_id = create_result.first;
            }
        }
        if (state_record_already_exists)
        {
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
                    err << "Reading r0_state record failed for id " << state_record_id << read_r0_state.second.error <<
                        commit;
                    return;
                }
                models::R0State r0_state;
                r0_state.from_values(read_r0_state.first);

                int old_repetitions = r0_state.repetitions;
                r0_state.repetitions = correct ? r0_state.repetitions + 1 : 0;
                // Interval is based on previous repetition count (before increment)
                r0_state.interval = get_next_r0_interval(old_repetitions, correct);

                r0_state.next_review = util::Utils::current_unix_timestamp_ms() + r0_state.interval *
                    MILLISECONDS_PER_DAY;
                r0_state.last_review = r_review.review_date;
                r0_state.last_quality = r_review.grade;
                auto new_values = r0_state.to_values();
                auto r0_state_update = run_update(*model_definition, token, state_record_id, new_values, stack_depth);

                if (r0_state_update.ko())
                {
                    action_result.status = 500;
                    action_result.error = read_r0_state.second.error;
                    err << "Updating r0_state record failed for id " << state_record_id << r0_state_update.error <<
                        commit;
                    return;
                }
            };
            break;
        case enums::RepetitionAlgorithm::Repetition2:
            {
                auto read_r2_state = run_read(*model_definition, token, state_record_id, stack_depth);
                if (read_r2_state.second.ko())
                {
                    action_result.status = 500;
                    action_result.error = read_r2_state.second.error;
                    err << "Reading r2_state record failed for id " << state_record_id << read_r2_state.second.error <<
                        commit;
                    return;
                }

                models::R2State r2_state;
                r2_state.from_values(read_r2_state.first);

                const int q = std::clamp(r_review.grade, 0, 5);


                double ef = r2_state.ef_times_100 / 100.0;
                int interval = r2_state.interval;
                int reps = r2_state.repetitions;

                if (q < 3)
                {
                    reps = 0;
                    interval = 1;
                }
                else
                {
                    if (reps == 0)
                        interval = 1;
                    else if (reps == 1)
                        interval = 6;
                    else
                        interval = static_cast<int>(std::round(interval * ef));

                    // Update EF (Easiness Factor)
                    ef = ef + (0.1 - (5 - q) * (0.08 + (5 - q) * 0.02));
                    if (ef < 1.3) ef = 1.3;

                    reps += 1;
                }

                // Update the state
                r2_state.repetitions = reps;
                r2_state.interval = interval;
                r2_state.ef_times_100 = static_cast<int>(std::round(ef * 100.0));
                r2_state.last_quality = q;
                r2_state.last_review = r_review.review_date;
                r2_state.next_review = util::Utils::current_unix_timestamp_ms() + interval * MILLISECONDS_PER_DAY;

                auto new_values = r2_state.to_values();
                auto r2_state_update = run_update(*model_definition, token, state_record_id, new_values, stack_depth);

                if (r2_state_update.ko())
                {
                    action_result.status = 500;
                    action_result.error = r2_state_update.error;
                    err << "Updating r2_state record failed for id " << state_record_id << r2_state_update.error <<
                        commit;
                    return;
                }

                debug << "SM2 review for note_id=" << r_review.note_id
                    << " q=" << q
                    << " reps=" << reps
                    << " ef=" << ef
                    << " interval=" << interval << "d"
                    << commit;
            };
            break;

        case enums::RepetitionAlgorithm::Repetition4:
            {
                auto read_r4_state = run_read(*model_definition, token, state_record_id, stack_depth);
                if (read_r4_state.second.ko())
                {
                    action_result.status = 500;
                    action_result.error = read_r4_state.second.error;
                    err << "Reading r4_state record failed for id " << state_record_id
                        << read_r4_state.second.error << commit;
                    return;
                }

                models::R4State r4_state;
                r4_state.from_values(read_r4_state.first);

                const int q = std::clamp(r_review.grade, 0, 5);
                

                double ef = r4_state.ef_times_100 / 100.0;
                double cf = r4_state.correction_factor_times_100 / 100.0;
                int reps = r4_state.repetitions;
                int interval = r4_state.interval;

                if (q < 3)
                {
                    // failure → reset
                    reps = 0;
                    interval = 1;
                    cf = 1.0;
                }
                else
                {
                    // update correction factor
                    cf = 1.0 + ((q - 3) * 0.05);
                    if (cf < 0.7) cf = 0.7;
                    if (cf > 1.5) cf = 1.5;

                    // update easiness factor (same formula as SM-2)
                    ef = ef + (0.1 - (5 - q) * (0.08 + (5 - q) * 0.02));
                    if (ef < 1.3) ef = 1.3;

                    if (reps == 0)
                        interval = 1;
                    else if (reps == 1)
                        interval = 6;
                    else
                        interval = static_cast<int>(std::round(interval * ef * cf));

                    reps += 1;
                }

                // save back
                r4_state.repetitions = reps;
                r4_state.interval = interval;
                r4_state.ef_times_100 = static_cast<int>(std::round(ef * 100.0));
                r4_state.correction_factor_times_100 = static_cast<int>(std::round(cf * 100.0));
                r4_state.last_quality = q;
                r4_state.last_review = r_review.review_date;
                r4_state.next_review = util::Utils::current_unix_timestamp_ms() + interval * MILLISECONDS_PER_DAY;

                auto new_values = r4_state.to_values();
                auto r4_state_update = run_update(*model_definition, token, state_record_id, new_values, stack_depth);

                if (r4_state_update.ko())
                {
                    action_result.status = 500;
                    action_result.error = r4_state_update.error;
                    err << "Updating r4_state record failed for id " << state_record_id
                        << r4_state_update.error << commit;
                    return;
                }

                debug << "SM4 review for note_id=" << r_review.note_id
                    << " q=" << q
                    << " reps=" << reps
                    << " ef=" << ef
                    << " cf=" << cf
                    << " interval=" << interval << "d"
                    << commit;
            };
            break;

        case enums::RepetitionAlgorithm::Repetition18:
            {
                auto read_r18_state = run_read(*model_definition, token, state_record_id, stack_depth);
                if (read_r18_state.second.ko())
                {
                    action_result.status = 500;
                    action_result.error = read_r18_state.second.error;
                    err << "Reading r18_state record failed for id " << state_record_id
                        << read_r18_state.second.error << commit;
                    return;
                }

                models::R18State r18_state;
                r18_state.from_values(read_r18_state.first);

                const int q = std::clamp(r_review.grade, 0, 5);

                // =======================
                // Model parameters (SM-18)
                // =======================
                const double b = 0.6;
                const double R_target = 0.9;
                const double R_opt = 0.9;
                const double alpha = 0.3;
                const double beta = 0.6;
                const double gamma = 0.2;
                const double delta = 0.4;
                const double k_over = 0.15;
                const double S_min = 0.5;
                const double short_retry = 0.5;
                const double t0 = 0.2;
                const double R_inf = 0.02;
                const double fatigue_lambda = 0.1;
                const double theta = 1.0; // user sensitivity

                double S = r18_state.stability_times_100 / 100.0;
                int reps = r18_state.repetitions;
                int lapses = r18_state.lapses;
                const double now_ms = (double)util::Utils::current_unix_timestamp_ms();
                const double elapsed_days = std::max(
                    0.0, (now_ms - (double)r18_state.last_review) / MILLISECONDS_PER_DAY);

                // =======================
                // New retrievability with offset and asymptote
                // =======================
                auto retrievability = [&](double t, double Sval)
                {
                    if (Sval <= 1e-9) Sval = 1e-9;
                    double x = (t + t0) / Sval;
                    double base = std::exp(-std::pow(std::max(0.0, x), b));
                    return R_inf + (1.0 - R_inf) * base;
                };

                auto interval_for_target = [&](double Sval, double Rval)
                {
                    double val = Sval * std::pow(-std::log(std::max(1e-9, Rval)), 1.0 / b);
                    return std::clamp(val, 0.1, 36500.0);
                };

                // =======================
                // Compute retrievability and overdue
                // =======================
                double R_now = retrievability(elapsed_days, S);
                double I_opt = interval_for_target(S, R_opt);
                double overdue = std::max(0.0, elapsed_days / std::max(1e-9, I_opt) - 1.0);
                double g_over = 1.0 + k_over * overdue;

                // =======================
                // Stability (with user sensitivity)
                // =======================
                double S_before = S;
                double S_after = S_before;

                if (q >= 3)
                {
                    double gain = alpha * (q == 3 ? 0.9 : q == 4 ? 1.0 : 1.1)
                        * std::pow((1.0 - R_now), beta)
                        * g_over;

                    double user_factor = std::pow(theta, 0.5);
                    S_after = S_before * (1.0 + gain * user_factor);
                }
                else
                {
                    double loss = gamma * std::pow(R_now, delta);
                    S_after = std::max(S_min, S_before * (1.0 - loss));
                    lapses += 1;
                    reps = 0;
                }

                // =======================
                // Adaptive interval with fatigue penalty
                // =======================
                auto fatigue = [&](double t) { return std::max(0.0, 1.0 - std::exp(-t / 2.0)); };

                double next_interval_days = (q >= 3)
                                                ? interval_for_target(S_after, R_target) * (1.0 + fatigue_lambda *
                                                    fatigue(elapsed_days))
                                                : short_retry;

                next_interval_days = std::clamp(next_interval_days, 0.1, 3650.0);

                // =======================
                // Update state
                // =======================
                r18_state.repetitions = reps + (q >= 3 ? 1 : 0);
                r18_state.lapses = lapses;
                r18_state.last_quality = q;
                r18_state.last_review = (int64_t)now_ms;
                r18_state.stability_times_100 = (int)std::round(S_after * 100.0);
                r18_state.last_interval_times_100 = (int)std::round(next_interval_days * 100.0);
                r18_state.next_review = (int64_t)(now_ms + next_interval_days * MILLISECONDS_PER_DAY);

                // =======================
                // Write to database
                // =======================
                auto new_values = r18_state.to_values();
                auto r18_state_update = run_update(*model_definition, token, state_record_id, new_values, stack_depth);

                if (r18_state_update.ko())
                {
                    action_result.status = 500;
                    action_result.error = r18_state_update.error;
                    err << "Updating r18_state record failed for id " << state_record_id
                        << r18_state_update.error << commit;
                    return;
                }

                debug << "SM18+ review for note_id=" << r_review.note_id
                    << " q=" << q
                    << " reps=" << r18_state.repetitions
                    << " S_before=" << S_before
                    << " S_after=" << S_after
                    << " R_now=" << R_now
                    << " interval=" << next_interval_days << "d"
                    << " overdue=" << overdue
                    << " fatigue_lambda=" << fatigue_lambda
                    << " theta=" << theta
                    << commit;
            };
            break;
        }
    }
}
