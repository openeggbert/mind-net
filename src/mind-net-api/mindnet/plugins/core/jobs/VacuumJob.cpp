//
// Created by robertvokac on 11/15/25.
//

#include "mindnet/plugins/core/jobs/VacuumJob.hpp"

#include "mindnet/essential/Global.hpp"
#include "mindnet/util/Utils.hpp"

namespace mindnet::plugins::core::jobs
{
    using_loggers()

    VacuumJob::VacuumJob()
        : Job(
            "VacuumJob",
            "VacuumJob",
            "@monthly",
            false,
            true
        )
    {

    }
    std::string VacuumJob::run(api::cronq::JobConfig& job_config)
    {
        essential::info << "Started VacuumJob" << essential::commit;

        nlohmann::json req;

        nlohmann::json result = call_query("Vacuum", req);

        return result.contains("error") ? result["error"] : "";
    }
    using_loggers()
}
