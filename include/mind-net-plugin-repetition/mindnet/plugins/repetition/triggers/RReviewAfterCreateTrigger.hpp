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

#pragma once



#include "mindnet/api/Trigger.hpp"
#include "mindnet/plugins/repetition/models/RReview.hpp"


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
        void run_before_or_after(
            mindnet::essential::Crudl operation,
            int stack_depth,
            api::OperationResult& validation_result,
            api::OperationResult& action_result,
            const model::ModelDefinition def,
            identification user_id,
            identification id,
            entity_fields& fields,
            entity_fields& old_fields,
            const orm::QueryParams query_params
        ) override;

    private:
        std::optional<double> fetch_user_param(
            identification user_id,
            const std::string& key,
            mindnet::api::AccessTokenContext& token,
            int stack_depth);

        std::optional<double> fetch_global_param(
            const std::string& key,
            api::AccessTokenContext& token,
            int stack_depth);

        double get_param(
            identification user_id,
            const std::string& key,
            double def,
            api::AccessTokenContext& token,
            int stack_depth);
        Params load_params_once(identification user_id, api::AccessTokenContext& token, int stack_depth);
    };
}
