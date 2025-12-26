/*
 * MIT License
 * Copyright (c) 2025 Robert Vokac
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include "mindnet/plugins/dictionary/triggers/DictionaryReviewAfterCreateTrigger.hpp"

#include <shared_mutex>

#include "mindnet/essential/Global.hpp"
#include "mindnet/api/AccessTokenContext.hpp"
#include "mindnet/plugins/core/models/AuthLog.hpp"
#include "mindnet/plugins/dictionary/models/DictionaryReview.hpp"
#include "mindnet/plugins/dictionary/models/DictionaryState18.hpp"
#include "mindnet/plugins/dictionary/models/DictionaryTerm.hpp"
#include "mindnet/util/Utils.hpp"

namespace mindnet::plugins::dictionary::triggers
{
    using_loggers()

    constexpr static int64_t MILLISECONDS_PER_DAY = 86400L * 1000;

    DictionaryReviewAfterCreateTrigger::DictionaryReviewAfterCreateTrigger()
        : Trigger(
            "DictionaryReviewAfterCreateTrigger",
            "Inserts or updates a state table after a successful creation of a review.",
            1000,
            {essential::Crudl::Create},
            api::TriggerPhase::After,
            "dictionary_review"
        )
    {
    }

    bool is_algorithm_supported(
        api::OperationResult& validation_result,
        const dictionary::models::DictionaryReview& review
    )
    {
        switch (review.algorithm)
        {
        // case enums::RepetitionAlgorithm::Repetition0:
        //     break;
        // case enums::RepetitionAlgorithm::Repetition2:
        //     break;
        // case enums::RepetitionAlgorithm::Repetition4:
        //     break;
        case enums::RepetitionAlgorithm::Repetition18:
            break;
        default:
            {
                validation_result = {400, "Unsupported algorithm."};
                return false;
            }
        }
        return true;
    }

    namespace params
    {
        inline constexpr double B = 1.1;
        inline constexpr double R_TARGET = 0.82;
        inline constexpr double R_OPT = 0.75;
        inline constexpr double ALPHA = 0.5;
        inline constexpr double BETA = 1.05;
        inline constexpr double GAMMA = 0.2;
        inline constexpr double DELTA = 0.4;
        inline constexpr double K_OVER = 0.55;
        inline constexpr double S_MIN = 8.0;
        inline constexpr double SHORT_RETRY = 0.02;
        inline constexpr double T0 = 0.2;
        inline constexpr double R_INFTY = 0.02;
        inline constexpr double FATIGUE_LAMBDA = 0.1;
        inline constexpr double THETA = 1.0;
        inline constexpr double G_OVER_MAX = 4.0;
        inline constexpr double S_DAMP = 8000.0;
        inline constexpr double MAX_GAIN = 2.0;
        inline constexpr double INTERVAL_SCALE = 2.2;
        inline constexpr double GROWTH_CAP = 5.0;
        inline constexpr double MIN_INTERVAL_DAYS = 1.0;
        inline constexpr double EF_MAX = 2.6;

        inline constexpr double MAX_INTERVAL_DAYS = 1825.0;
    }

    void DictionaryReviewAfterCreateTrigger::run_before_or_after(
        mindnet::essential::Crudl operation,
        int stack_depth,
        api::OperationResult& validation_result,
        api::OperationResult& action_result,
        const mindnet::model::ModelDefinition def,
        identification user_id,
        identification id,
        entity_fields& fields,
        entity_fields& old_fields,
        const orm::QueryParams query_params)
    {
        if (action_result.ko())
        {
            return;
        }

        models::DictionaryReview review;
        review.from_values(fields);

        auto token = api::AccessTokenContext(user_id, "", 200);

        if (!is_algorithm_supported(validation_result, review)) return;

        identification state_record_id{};

        // ============================================================
        // Checking, if the state record already exists.
        // ============================================================

        orm::QueryParams params;
        params.add_filter("user_id", review.user_id);
        params.add_filter("dictionary_term_id", review.dictionary_term_id);

        auto& state_def = models::DICTIONARY_STATE_18_DEFINITION;
        auto list_result = run_list(state_def, token, params, stack_depth);
        if (list_result.second.ko())
        {
            validation_result.status = 500;
            validation_result.error = list_result.second.error;
            err << "Listing table " << state_def.get_model_name() << " failed for user_id " << review.user_id
                << " and dictionary_term_id " << review.dictionary_term_id << list_result.second.error << std::endl;
            return;
        }
        bool state_record_already_exists = !list_result.first.empty();

        // ============================================================
        // Creating new state record, if it does not yet exist.
        // ============================================================

        int64_t now = static_cast<int64_t>(util::Utils::current_unix_timestamp_ms());

        bool default_state_record_will_be_created = !state_record_already_exists;
        if (default_state_record_will_be_created)
        {
            entity_fields new_state_fields;


            switch (review.algorithm)
            {
            case enums::RepetitionAlgorithm::Repetition18:
                {
                    models::DictionaryState18 state;
                    state.user_id = review.user_id;
                    state.dictionary_term_id = review.dictionary_term_id;

                    state.stability_times_100 = 800;
                    state.last_interval_times_100 = 0;
                    state.repetitions = 0;
                    state.lapses = 0;

                    state.next_review = 0;
                    state.last_review = 0;
                    state.last_quality = 0;

                    new_state_fields = state.to_values();
                }
                break;
            default: throw std::runtime_error { std::string("Unsupported algorithm: ") + std::to_string(static_cast<int>(review.algorithm))};
            }

            new_state_fields[1] = now;
            new_state_fields[2] = now;

            auto create_result = run_create(state_def, token, new_state_fields, stack_depth);

            if (create_result.second.ko())
            {
                validation_result.status = 500;
                validation_result.error = create_result.second.error;
                err << "Creating new state table " << state_def.get_model_name() << " failed for user_id " <<
                    review.user_id << " and dictionary_term_id " << review.dictionary_term_id << " " << create_result.
                    second.error <<
                    commit;
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
            identification* id_tmp = std::get_if<int64_t>(&entity_fields_[0]);
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
        // --- tunable safety caps (user/global settings) ---

        auto read_term = run_read(models::DICTIONARY_TERM_DEFINITION, token, review.dictionary_term_id, stack_depth);
        if (read_term.second.ko())
        {
            validation_result.status = 500;
            validation_result.error = read_term.second.error;
            err << "Reading dictionary_term record failed for id " << state_record_id
                << read_term.second.error << commit;
            return;
        }
        models::DictionaryTerm term;
        term.from_values(read_term.first);

        model::JSON details_json;

        switch (review.algorithm)
        {
        case enums::RepetitionAlgorithm::Repetition18:
            {
                auto read_r18_state = run_read(state_def, token, state_record_id, stack_depth);
                if (read_r18_state.second.ko())
                {
                    validation_result.status = 500;
                    validation_result.error = read_r18_state.second.error;
                    err << "Reading r18_state record failed for id " << state_record_id
                        << read_r18_state.second.error << commit;
                    return;
                }

                models::DictionaryState18 r18_state;
                r18_state.from_values(read_r18_state.first);
                if (r18_state.next_review > now)
                {
                    //future
                    debug << "Review is in future for state18 with id " << state_record_id << commit;
                    return;
                }

                const int q = std::clamp(review.grade, 0, 5);
                bool was_correct = q >= 3;

                // =======================
                // Model parameters (SM-18)
                // =======================

                constexpr double b = params::B;
                constexpr double R_target = params::R_TARGET;
                constexpr double R_opt = params::R_OPT;
                constexpr double alpha = params::ALPHA;
                constexpr double beta = params::BETA;
                constexpr double gamma = params::GAMMA;
                constexpr double delta = params::DELTA;
                constexpr double k_over = params::K_OVER;
                constexpr double S_min = params::S_MIN;
                constexpr double short_retry = params::SHORT_RETRY;
                constexpr double t0 = params::T0;
                constexpr double R_inf = params::R_INFTY;
                constexpr double fatigue_lambda = params::FATIGUE_LAMBDA;
                constexpr double theta = params::THETA;
                constexpr double G_OVER_MAX = params::G_OVER_MAX;
                constexpr double S_DAMP = params::S_DAMP;
                constexpr double MAX_GAIN = params::MAX_GAIN;
                double interval_scale = params::INTERVAL_SCALE;
                constexpr double min_interval_days = params::MIN_INTERVAL_DAYS;

                if (r18_state.stability_times_100 < S_min * 100.0)
                {
                    r18_state.stability_times_100 = S_min * 100.0;
                }
                double S = r18_state.stability_times_100 / 100.0;
                int reps = r18_state.repetitions;
                int lapses = r18_state.lapses;

                // --- deterministic time based on review_date
                const double now_ms = static_cast<double>(
                    review.review_date > 0 ? review.review_date : util::Utils::current_unix_timestamp_ms());

                // --- protection for first review
                double elapsed_days = 0.0;
                if (r18_state.last_review > 0)
                {
                    elapsed_days = std::max(
                        0.0, (now_ms - static_cast<double>(r18_state.last_review)) / MILLISECONDS_PER_DAY);
                }
                else
                {
                    // fallback: use previous intervals or small seed
                    elapsed_days = (r18_state.last_interval_times_100 > 0)
                                       ? static_cast<double>(r18_state.last_interval_times_100) / 100.0
                                       : 0.1;
                }

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
                    return std::clamp(val, 0.1, 3650.0);
                };

                // =======================
                // Compute retrievability and overdue
                // =======================
                double R_now = retrievability(elapsed_days, S);
                details_json["R_now"] = R_now;
                double I_opt = interval_for_target(S, R_opt);
                double overdue = std::max(0.0, elapsed_days / std::max(1e-9, I_opt) - 1.0);

                double g_over = 1.0 + k_over * overdue;

                g_over = std::clamp(g_over, 0.0, G_OVER_MAX);
                // =======================
                // Stability (with user sensitivity)
                // =======================
                double S_before = S;
                double S_after = S_before;

                if (q >= 3)
                {
                    double gain = alpha * 1.1
                        * std::pow((1.0 - R_now), beta)
                        * g_over;

                    double damp = 1.0 / (1.0 + std::pow(S_before / std::max(1e-9, S_DAMP), 0.5));
                    gain *= damp;

                    gain = std::clamp(gain, 0.0, MAX_GAIN);

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

                double base_interval = interval_for_target(S_after, R_target);

                // fatigue only for wrong answers
                double fatigue_multiplier = (q < 3)
                                                ? (1.0 + fatigue_lambda * fatigue(elapsed_days))
                                                : 1.0;

                double next_interval_days = (q >= 3)
                                                ? base_interval * fatigue_multiplier
                                                : short_retry;

                next_interval_days *= interval_scale;

                if (q >= 3 && r18_state.last_interval_times_100 > 0)
                {
                    const double growth_cap = params::GROWTH_CAP; // OK
                    double last_days = r18_state.last_interval_times_100 / 100.0;
                    next_interval_days = std::min(next_interval_days, last_days * growth_cap);
                }

                // =======================
                // Update state
                // =======================

                if (!std::isfinite(S_after)) S_after = std::max(S_min, 1.0);
                S_after = std::clamp(S_after, S_min, 1e6);

                next_interval_days = std::clamp(next_interval_days, min_interval_days, 3650.0);

                if (q >= 4)
                {
                    double difficulty_factor = 1.0;
                    enums::Difficulty difficulty = static_cast<enums::Difficulty>(term.difficulty);
                    if (difficulty == enums::Difficulty::Easy) difficulty_factor = 0.8;
                    if (difficulty == enums::Difficulty::Medium) difficulty_factor = 1.0;
                    if (difficulty == enums::Difficulty::Hard) difficulty_factor = 1.25;
                    next_interval_days = next_interval_days / difficulty_factor;
                }

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
                auto r18_state_update = run_update(state_def, token, state_record_id, new_values, stack_depth);

                if (r18_state_update.ko())
                {
                    validation_result.status = 500;
                    validation_result.error = r18_state_update.error;
                    err << "Updating r18_state record failed for id " << state_record_id
                        << r18_state_update.error << commit;
                    return;
                }

                debug << "SM18+ review for dictionary_term_id=" << review.dictionary_term_id
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

                {
                    double R_pred = R_now; // retrievability before review
                    int R_pred_times_100 = (int)(R_pred * 100.0);
                }
            } break;
        default: throw std::runtime_error{
                std::string("Unsupported algorithm: ") + std::to_string(static_cast<int>(review.algorithm))
            };
        }

        if (!details_json.empty())
        {
            review.details_json = details_json.dump();
            auto v = review.to_values();
            v[0] = id;
            auto review_updated = run_update(models::DICTIONARY_REVIEW_DEFINITION, token, id, v, stack_depth);
            if (review_updated.ko())
            {
                err << "Update of review with id " << review.get_id() << " failed: " << review_updated.error <<
                    commit;
            }
        }
    }
}
