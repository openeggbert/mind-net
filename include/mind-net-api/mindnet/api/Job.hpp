//
// Created by robertvokac on 9/7/25.
//
#ifndef JOB_HPP
#define JOB_HPP

#include <memory>
#include <nlohmann/json.hpp>
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
            const bool run_once_when_missed_ = true
            ):
        AbstractTriggerJob(job_name_, job_description_),
        cron_expression(cron_expression_),
        run_once_when_missed(run_once_when_missed_)
        {
        }

        virtual ~Job() = default;

        virtual std::string run() = 0;
        [[nodiscard]] const std::string& get_cron_expression() const { return cron_expression; }
        [[nodiscard]] bool get_run_once_when_missed() const { return run_once_when_missed; }
        void set_plugin_name(const std::string& plugin_name_) {plugin_name = plugin_name_;};
        [[nodiscard]] std::string get_plugin_name() const {return plugin_name;}

    private:
        std::string cron_expression;
        bool run_once_when_missed = true;
        std::string plugin_name;

    };


    typedef std::shared_ptr<Job> JobPtr;
}

#endif // JOB_HPP
