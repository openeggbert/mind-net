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
            "@monthly",
            false,
            true
        )
    {

    }
    std::string TestJob::run(api::cronq::JobConfig& job_config)
    {
        essential::err << "[TestJob] START" << essential::commit;
        auto content = job_config.get_string_or_default("content", "hi");
        essential::info << "Hello TestJob (" << content.first << ") "<< util::Utils::current_unixtime_to_string() << essential::commit;
        essential::err << "[TestJob] END" << essential::commit;
        return "";
    }
    using_loggers()
}
