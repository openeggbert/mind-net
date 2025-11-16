//
// Created by robertvokac on 11/15/25.
//

#include "mindnet/plugins/core/jobs/TestJob.hpp"

#include "mindnet/util/Utils.hpp"

namespace mindnet::plugins::core::jobs
{
    TestJob::TestJob()
        : Job(
            "TestJob",
            "For tests",
            "@every_seconds(15)",
            false,
            true
        )
    {

    }
    std::string TestJob::run(api::cronq::JobConfig& job_config)
    {
        auto content = job_config.get_string_or_default("content", "hi");
        essential::info << "Hello TestJob (" << content.first << ") "<< util::Utils::current_unixtime_to_string() << essential::commit;
        return "";
    }
    using_loggers()
}
