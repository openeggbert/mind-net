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
#include "mindnet/plugins/dictionary/models/DictionaryState4.hpp"
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
        case enums::RepetitionAlgorithm::Repetition4:
            break;
        // case enums::RepetitionAlgorithm::Repetition18:
        //     break;
        default:
            {
                validation_result = {400, "Unsupported algorithm."};
                return false;
            }
        }
        return true;
    }

    static const constexpr double EF_MAX = 2.6;

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

        auto& state_def = models::DICTIONARY_STATE_4_DEFINITION;
        auto list_result = run_list(state_def, token, params, stack_depth);
        if (list_result.second.ko())
        {
            action_result.status = 500;
            action_result.error = list_result.second.error;
            err << "Listing table " << state_def.get_model_name() << " failed for user_id " << review.user_id
                << " and dictionary_term_id " << review.dictionary_term_id << list_result.second.error << std::endl;
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

            switch (review.algorithm)
            {
            case enums::RepetitionAlgorithm::Repetition4:
                {
                    models::DictionaryState4 state;
                    state.user_id = review.user_id;
                    state.dictionary_term_id = review.dictionary_term_id;

                    state.repetitions = 0;
                    state.interval = 1;
                    state.ef_times_100 = 250;
                    state.correction_factor_times_100 = 100;

                    state.next_review = 0;
                    state.last_review = 0;
                    state.last_quality = 0;

                    new_state_fields = state.to_values();
                }
                break;
            }

            new_state_fields[1] = now;
            new_state_fields[2] = now;

            auto create_result = run_create(state_def, token, new_state_fields, stack_depth);

            if (create_result.second.ko())
            {
                action_result.status = 500;
                action_result.error = create_result.second.error;
                err << "Creating new state table " << state_def.get_model_name() << " failed for user_id " <<
                    review.user_id << " and dictionary_term_id " << review.dictionary_term_id << " " << create_result.second.error <<
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

        const int MAX_INTERVAL_DAYS = 3650;

        model::JSON details_json;

        switch (review.algorithm)
        {

        case enums::RepetitionAlgorithm::Repetition4:
            {
                auto read_r4_state = run_read(state_def, token, state_record_id, stack_depth);
                if (read_r4_state.second.ko())
                {
                    action_result.status = 500;
                    action_result.error = read_r4_state.second.error;
                    err << "Reading r4_state record failed for id " << state_record_id
                        << read_r4_state.second.error << commit;
                    return;
                }

                models::DictionaryState4 r4_state;
                r4_state.from_values(read_r4_state.first);

                // Correction Factor tuning
                constexpr double CF_GAIN = 0.025;
                // default gentler than 0.05
                constexpr double CF_MIN = 0.9;
                constexpr double CF_MAX = 1.1;

                const int q = std::clamp(review.grade, 0, 5);

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
                    cf = 1.0 + (q - 3) * CF_GAIN;
                    if (cf < CF_MIN) cf = CF_MIN;
                    if (cf > CF_MAX) cf = CF_MAX;

                    // update easiness factor (SM-2 formula) + clamp
                    ef = ef + (0.1 - (5 - q) * (0.08 + (5 - q) * 0.02));
                    if (ef < 1.3) ef = 1.3;
                    if (ef > EF_MAX) ef = EF_MAX;

                    if (reps == 0)
                        interval = 1;
                    else if (reps == 1)
                        interval = 6;
                    else
                        interval = static_cast<int>(std::round(interval * ef * cf));

                    reps += 1;
                }

                // --- clamp interval globally ---
                if (interval < 1) interval = 1;
                if (interval > MAX_INTERVAL_DAYS) interval = MAX_INTERVAL_DAYS;

                // save back
                r4_state.repetitions = reps;
                r4_state.interval = interval;
                r4_state.ef_times_100 = static_cast<int>(std::round(ef * 100.0));
                r4_state.correction_factor_times_100 = static_cast<int>(std::round(cf * 100.0));
                r4_state.last_quality = q;
                r4_state.last_review = review.review_date;
                r4_state.next_review = util::Utils::current_unix_timestamp_ms() + interval * MILLISECONDS_PER_DAY;

                auto new_values = r4_state.to_values();
                auto r4_state_update = run_update(state_def, token, state_record_id, new_values, stack_depth);

                if (r4_state_update.ko())
                {
                    action_result.status = 500;
                    action_result.error = r4_state_update.error;
                    err << "Updating state 4 record failed for id " << state_record_id
                        << r4_state_update.error << commit;
                    return;
                }

                debug << "SM4 review for dictionary_term_id=" << review.dictionary_term_id
                    << " q=" << q
                    << " reps=" << reps
                    << " ef=" << ef
                    << " cf=" << cf
                    << " interval=" << interval << "d"
                    << commit;
            };
            break;


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