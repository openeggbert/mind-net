//
// Created by robertvokac on 11/15/25.
//

#include "mindnet/plugins/core/jobs/CleanupHistoryOrphansJob.hpp"

#include "mindnet/util/Utils.hpp"

namespace mindnet::plugins::core::jobs
{
    CleanupHistoryOrphansJob::CleanupHistoryOrphansJob(void)
        : Job(
            "CleanupHistoryOrphansJob",
            "CleanupHistoryOrphansJob",
            "@monthly",
            true,
            true
        )
    {

    }
    std::string CleanupHistoryOrphansJob::run(api::cronq::JobConfig& job_config)
    {
        auto history_orphan_threshold_in_days = job_config.get_int_or_default("history_orphan_threshold_in_days", 30);


        nlohmann::json req;
        req["history_orphan_threshold_in_days"] = cast64(history_orphan_threshold_in_days.first);

        nlohmann::json result = call_query("CleanupHistoryOrphans", req);

        return result.contains("error") ? result["error"] : "";
    }
}
