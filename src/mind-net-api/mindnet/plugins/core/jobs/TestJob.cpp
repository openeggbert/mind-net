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
            "@every_seconds(3)",
            true
        )
    {

    }
    std::string TestJob::run()
    {
        essential::info << "Hello TestJob " << util::Utils::current_unixtime_to_string() << essential::commit;
        return "";
    }
    using_loggers()
}
