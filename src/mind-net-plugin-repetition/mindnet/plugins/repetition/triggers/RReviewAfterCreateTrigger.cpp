//
// Created by robertvokac on 9/7/25.
//

#include "mindnet/plugins/repetition/triggers/RReviewAfterCreateTrigger.h"

#include <shared_mutex>

#include "mindnet/essential/Global.h"
#include "mindnet/api/AccessTokenContext.h"
#include "mindnet/plugins/core/models/AuthLog.h"
#include "mindnet/plugins/repetition/models/R0State.h"
#include "mindnet/plugins/repetition/models/R18PerfAgg.h"
#include "mindnet/plugins/repetition/models/R18PredictionLog.h"
#include "mindnet/plugins/repetition/models/R18State.h"
#include "mindnet/plugins/repetition/models/R2State.h"
#include "mindnet/plugins/repetition/models/R4State.h"
#include "mindnet/plugins/repetition/models/RGlobalSetting.h"
#include "mindnet/plugins/repetition/models/RUserSetting.h"
#include "mindnet/plugins/repetition/models/RReview.h"
#include "mindnet/plugins/repetition/models/RSession.h"
#include "mindnet/util/Utils.h"

// ============================================================
// PARAMETER RESOLUTION (r_user_setting -> r_global_setting -> default)
// ============================================================

namespace
{
    struct ParamKey
    {
        int user_id;
        std::string key;
    };

    struct ParamKeyHash
    {
        size_t operator()(ParamKey const& k) const noexcept
        {
            return std::hash<long long>{}((static_cast<long long>(k.user_id) << 32) ^ std::hash<std::string>{}(k.key));
        }
    };

    struct ParamKeyEq
    {
        bool operator()(ParamKey const& a, ParamKey const& b) const noexcept
        {
            return a.user_id == b.user_id && a.key == b.key;
        }
    };

    static std::unordered_map<ParamKey, double, ParamKeyHash, ParamKeyEq> g_user_param_cache;
    static std::unordered_map<std::string, double> g_global_param_cache;
    static std::shared_mutex g_user_param_mutex;
    static std::shared_mutex g_global_param_mutex;
} // namespace

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
        case enums::RepetitionAlgorithm::Repetition4: model_definition = &models::R4_STATE_DEFINITION;
            break;
        case enums::RepetitionAlgorithm::Repetition18: model_definition = &models::R18_STATE_DEFINITION;
            break;
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
        const static std::vector<int> SM0_INTERVALS = {1, 6, 16, 35, 62, 100, 150, 210, 300, 420};

        if (!correct) return SM0_INTERVALS[0];
        int next_index = std::min(previous_repetitions, static_cast<int>(SM0_INTERVALS.size()) - 1);
        return SM0_INTERVALS[next_index];
    }

    std::optional<double> RReviewAfterCreateTrigger::fetch_user_param(
        int user_id,
        const std::string& key,
        mindnet::api::AccessTokenContext& token,
        int stack_depth)
    {
        ParamKey pk{user_id, key};
        {
            // 🔒 read lock
            std::shared_lock lock(g_user_param_mutex);
            if (auto it = g_user_param_cache.find(pk); it != g_user_param_cache.end())
                return it->second;
        }

        mindnet::orm::QueryParams qp;
        qp.add_filter("user_id", user_id);
        qp.add_filter("key", key);
        auto res = run_list(models::R_USER_SETTING_DEFINITION, token, qp, stack_depth);
        if (res.second.ok() && !res.first.empty())
        {
            const auto& row = res.first.front();
            models::RUserSetting setting;
            setting.from_values(row);

            double val{};
            try { val = std::stod(setting.value); }
            catch (...)
            {
                return std::nullopt;
            }
            {
                // 🔒 write lock
                std::unique_lock lock(g_user_param_mutex);
                g_user_param_cache.emplace(pk, val);
            }
            return val;
        }
        return std::nullopt;
    }

    std::optional<double> RReviewAfterCreateTrigger::fetch_global_param(
        const std::string& key,
        api::AccessTokenContext& token,
        int stack_depth)
    {
        {
            // 🔒 read lock
            std::shared_lock lock(g_global_param_mutex);
            if (auto it = g_global_param_cache.find(key); it != g_global_param_cache.end())
                return it->second;
        }

        orm::QueryParams qp;
        qp.add_filter("key", key);
        auto res = run_list(models::R_GLOBAL_SETTING_DEFINITION, token, qp, stack_depth);
        if (res.second.ok() && !res.first.empty())
        {
            const auto& row = res.first.front();
            models::RGlobalSetting setting;
            setting.from_values(row);


            double val{};

            try { val = std::stod(setting.value); }
            catch (...)
            {
                return std::nullopt;
            }
            {
                // 🔒 write lock
                std::unique_lock lock(g_global_param_mutex);
                g_global_param_cache.emplace(key, val);
            }
            return val;
        }
        return std::nullopt;
    }

    double RReviewAfterCreateTrigger::get_param(
        int user_id,
        const std::string& key,
        double def,
        api::AccessTokenContext& token,
        int stack_depth)
    {
        if (auto u = fetch_user_param(user_id, key, token, stack_depth)) return *u;
        if (auto g = fetch_global_param(key, token, stack_depth)) return *g;
        return def;
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

        // ============================================
        // Skip updates for "All" sessions
        // ============================================
        if (r_review.r_session_id > 0)
        {
            // load r_session to determine scope
            auto read_rsession = run_read(models::R_SESSION_DEFINITION, token, r_review.r_session_id, stack_depth);
            if (read_rsession.second.ok())
            {
                models::RSession session;
                session.from_values(read_rsession.first);
                // 3 = Scope::All
                if (session.scope == enums::RepetitionScope::All)
                {
                    debug << "Skipping state update for note_id=" << r_review.note_id
                        << " (scope=All)" << commit;
                    return;
                }
            }
            if (read_rsession.second.ko())
            {
                err << "Reading r_session with ID " << r_review.r_session_id << " failed: " << read_rsession.second.
                    error << commit;
                return;
            }
        }

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

                    state.stability_times_100 = 400;
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
        // --- tunable safety caps (user/global settings) ---
        const double EF_MAX = get_param(user_id, "ef_max", 2.6, token, stack_depth); // upper limit of EF
        const int MAX_INTERVAL_DAYS = (int)std::round(
            get_param(user_id, "max_interval_days", 1825.0, token, stack_depth)); // max 5 years

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

                    // clamp EF to [1.3, EF_MAX]
                    if (ef < 1.3) ef = 1.3;
                    if (ef > EF_MAX) ef = EF_MAX;

                    reps += 1;
                }

                // --- clamp interval globally ---
                if (interval < 1) interval = 1;
                if (interval > MAX_INTERVAL_DAYS) interval = MAX_INTERVAL_DAYS;


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

                // Correction Factor tuning
                const double CF_GAIN = get_param(user_id, "cf_gain", 0.025, token, stack_depth);
                // default jemnější než 0.05
                const double CF_MIN = get_param(user_id, "cf_min", 0.9, token, stack_depth);
                const double CF_MAX = get_param(user_id, "cf_max", 1.1, token, stack_depth);

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
                bool was_correct = q >= 3;

                // =======================
                // Model parameters (SM-18)
                // =======================
                const double b = get_param(user_id, "b", 0.6, token, stack_depth);
                const double R_target = get_param(user_id, "R_target", 0.9, token, stack_depth);
                const double R_opt = get_param(user_id, "R_opt", 0.9, token, stack_depth);
                const double alpha = get_param(user_id, "alpha", 0.3, token, stack_depth);
                const double beta = get_param(user_id, "beta", 0.6, token, stack_depth);
                const double gamma = get_param(user_id, "gamma", 0.2, token, stack_depth);
                const double delta = get_param(user_id, "delta", 0.4, token, stack_depth);
                const double k_over = get_param(user_id, "k_over", 0.15, token, stack_depth);
                const double S_min = get_param(user_id, "S_min", 0.5, token, stack_depth);
                const double short_retry = get_param(user_id, "short_retry", 0.02, token, stack_depth);
                const double t0 = get_param(user_id, "t0", 0.2, token, stack_depth);
                const double R_inf = get_param(user_id, "R_infty", 0.02, token, stack_depth);
                const double fatigue_lambda = get_param(user_id, "fatigue_lambda", 0.1, token, stack_depth);
                const double theta = get_param(user_id, "theta", 1.0, token, stack_depth);

                double S = r18_state.stability_times_100 / 100.0;
                int reps = r18_state.repetitions;
                int lapses = r18_state.lapses;

                // --- deterministic time based on review_date
                const double now_ms = static_cast<double>(
                    r_review.review_date > 0 ? r_review.review_date : util::Utils::current_unix_timestamp_ms());

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

                // =======================
                // Update state
                // =======================

                if (!std::isfinite(S_after)) S_after = std::max(S_min, 1.0);
                S_after = std::clamp(S_after, S_min, 1e6);
                const double min_interval_days = get_param(user_id, "min_interval_days", 0.01, token, stack_depth);
                next_interval_days = std::clamp(next_interval_days, min_interval_days, 3650.0);

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


                {
                    double R_pred = R_now; // retrievability before review
                    int R_pred_times_100 = (int)(R_pred * 100.0);

                    models::R18PredictionLog r18_prediction_log;

                    r18_prediction_log.user_id = r_review.user_id;
                    r18_prediction_log.note_id = r_review.note_id;
                    r18_prediction_log.predicted_R_times_100 = R_pred_times_100;
                    r18_prediction_log.actual_grade = q;
                    r18_prediction_log.was_correct = was_correct;
                    auto plv = r18_prediction_log.to_values();
                    plv[1] = util::Utils::current_unix_timestamp_ms();
                    plv[2] = util::Utils::current_unix_timestamp_ms();
                    run_create(models::R18_PREDICTION_LOG_DEFINITION, token, plv, stack_depth);
                }
                {
                    int bin_log_t_times_100 = static_cast<int>(std::round(100.0 * std::log(elapsed_days + 1.0)));

                    orm::QueryParams qpa;
                    qpa.add_filter("user_id", r_review.user_id);
                    qpa.add_filter("bin_log_t_times_100", bin_log_t_times_100);
                    auto list = run_list(models::R18_PERF_AGG_DEFINITION, token, qpa, stack_depth);

                    if (list.second.ok())
                    {
                        if (list.first.empty())
                        {
                            models::R18PerfAgg perf;
                            perf.user_id = r_review.user_id;
                            perf.bin_log_t_times_100 = bin_log_t_times_100;
                            perf.total = 1;
                            perf.correct = was_correct ? 1 : 0;
                            auto v = perf.to_values();

                            v[1] = util::Utils::current_unix_timestamp_ms();
                            v[2] = util::Utils::current_unix_timestamp_ms();
                            run_create(models::R18_PERF_AGG_DEFINITION, token, v, stack_depth);
                        }
                        else
                        {
                            auto row = list.first.front();
                            models::R18PerfAgg perf;
                            perf.from_values(row);
                            perf.total += 1;
                            if (was_correct) perf.correct += 1;
                            auto v = perf.to_values();
                            v[2] = util::Utils::current_unix_timestamp_ms();
                            run_update(models::R18_PERF_AGG_DEFINITION, token, perf.get_id(), v, stack_depth);
                        }
                    }
                }
            };
            break;
        }
    }
}
