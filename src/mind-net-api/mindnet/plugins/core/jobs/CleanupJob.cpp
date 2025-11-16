//
// Created by robertvokac on 11/15/25.
//

#include "mindnet/plugins/core/jobs/CleanupJob.hpp"

#include "mindnet/util/Utils.hpp"

namespace mindnet::plugins::core::jobs
{
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
        auto content = job_config.get_string_or_default("content", "hi");
        essential::info << "Hello CleanupJob (" << content.first << ") "<< util::Utils::current_unixtime_to_string() << essential::commit;
        return "";
    }
    using_loggers()
}
