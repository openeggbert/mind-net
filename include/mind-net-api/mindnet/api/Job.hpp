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

#include <memory>
#include <nlohmann/json.hpp>

#include "cronq/JobConfig.hpp"
#include "mindnet/api/AbstractTriggerJob.hpp"

namespace mindnet::api
{
    class Job : public AbstractTriggerJob
    {
    public:
        Job(
            const std::string& job_name_,
            const std::string& job_description_,
            const std::string& cron_expression_,
            const bool enabled_by_default_ = true,
            const bool run_once_when_missed_ = true
            ):
        AbstractTriggerJob(job_name_, job_description_),
        cron_expression(cron_expression_),
        enabled_by_default(enabled_by_default_),
        run_once_when_missed(run_once_when_missed_)
        {
        }

        virtual ~Job() = default;

        virtual std::string run(cronq::JobConfig& job_config) = 0;
        [[nodiscard]] const std::string& get_cron_expression() const { return cron_expression; }
        [[nodiscard]] bool get_run_once_when_missed() const { return run_once_when_missed; }
        void set_plugin_name(const std::string& plugin_name_) {plugin_name = plugin_name_;};
        [[nodiscard]] std::string get_plugin_name() const {return plugin_name;}
        [[nodiscard]] bool get_enabled_by_default() const {return enabled_by_default;}
        nlohmann::json call_query(const std::string& query_name, nlohmann::json& request);
    private:
        std::string cron_expression;
        bool run_once_when_missed = true;
        std::string plugin_name;
        bool enabled_by_default = true;

    };

    typedef std::shared_ptr<Job> JobPtr;
}

