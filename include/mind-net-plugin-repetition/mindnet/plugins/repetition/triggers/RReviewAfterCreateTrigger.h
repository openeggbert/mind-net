//
// Created by robertvokac on 9/7/25.
//
#ifndef RREVIEWAFTERCREATETRIGGER_H
#define RREVIEWAFTERCREATETRIGGER_H


#include "mindnet/api/Trigger.h"
#include "mindnet/plugins/repetition/models/RReview.h"

namespace mindnet::plugins::repetition::triggers
{
    struct Params {
        double b, R_target, R_opt, alpha, beta, gamma, delta, k_over, S_min,
               short_retry, t0, R_inf, fatigue_lambda, theta, g_over_max,
               s_damp, max_gain, interval_scale, growth_cap, min_interval_days, ef_max;
        int max_interval_days;
    };

    class RReviewAfterCreateTrigger : public api::Trigger
    {
    public:
        RReviewAfterCreateTrigger();

        ~RReviewAfterCreateTrigger() override = default;
        void run(
            mindnet::essential::Crudl operation,
            int stack_depth,
            api::OperationResult& validation_result,
            api::OperationResult& action_result,
            const model::ModelDefinition def,
            int user_id,
            int id,
            entity_fields& fields,
            entity_fields& old_fields,
            const orm::QueryParams query_params
        );

    private:
        std::optional<double> fetch_user_param(
            int user_id,
            const std::string& key,
            mindnet::api::AccessTokenContext& token,
            int stack_depth);

        std::optional<double> fetch_global_param(
            const std::string& key,
            api::AccessTokenContext& token,
            int stack_depth);

        double get_param(
            int user_id,
            const std::string& key,
            double def,
            api::AccessTokenContext& token,
            int stack_depth);
        Params load_params_once(int user_id, api::AccessTokenContext& token, int stack_depth);
    };
}

#endif // RREVIEWAFTERCREATETRIGGER_H
