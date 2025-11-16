//
// Created by robertvokac on 9/7/25.
//
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

    private:
        std::string cron_expression;
        bool run_once_when_missed = true;
        std::string plugin_name;
        bool enabled_by_default = true;

    };


    typedef std::shared_ptr<Job> JobPtr;
}

