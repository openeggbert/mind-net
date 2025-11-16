//
// Created by robertvokac on 11/15/25.
//

#include "mindnet/plugins/core/jobs/CleanupJob.hpp"

#include "mindnet/essential/Global.hpp"
#include "mindnet/util/Utils.hpp"

namespace mindnet::plugins::core::jobs
{
    using_loggers()

    CleanupJob::CleanupJob()
        : Job(
            "CleanupJob",
            "CleanupJob",
            "@daily_at(02:30)",
            true,
            true
        )
    {

    }
    std::string CleanupJob::run(api::cronq::JobConfig& job_config)
    {
        essential::info << "CleanupJob TestJob (" << util::Utils::current_unixtime_to_string() << essential::commit;

        auto api_log_threshold_in_days = job_config.get_int_or_default("api_log_threshold_in_days", 90);
        auto history_read_threshold_in_days = job_config.get_int_or_default("history_read_threshold_in_days", 90);
        auto history_list_threshold_in_days = job_config.get_int_or_default("history_list_threshold_in_days", 90);
        auto access_token_threshold_in_days = job_config.get_int_or_default("access_token_threshold_in_days", 90);


        nlohmann::json req;
        req["api_log_threshold_in_days"] = cast64(api_log_threshold_in_days.first);
        req["history_read_threshold_in_days"] = cast64(history_read_threshold_in_days.first);
        req["history_list_threshold_in_days"] = cast64(history_list_threshold_in_days.first);
        req["access_token_threshold_in_days"] = cast64(access_token_threshold_in_days.first);

        nlohmann::json result = call_query("Cleanup", req);

        return result.contains("error") ? result["error"] : "";
    }
    using_loggers()
}
