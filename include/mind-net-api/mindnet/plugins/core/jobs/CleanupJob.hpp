//
// Created by robertvokac on 9/7/25.
//
#pragma once

#include "mindnet/api/Job.hpp"

namespace mindnet::plugins::core::jobs
{
    class CleanupJob : public api::Job
    {
    public:
        CleanupJob();

        ~CleanupJob() = default;

        std::string run(api::cronq::JobConfig& job_config) override;

    private:
    };
}
