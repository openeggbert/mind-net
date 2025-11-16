//
// Created by robertvokac on 9/7/25.
//
#pragma once

#include "mindnet/api/Job.hpp"

namespace mindnet::plugins::core::jobs
{
    class VacuumJob : public api::Job
    {
    public:
        VacuumJob();

        ~VacuumJob() = default;

        std::string run(api::cronq::JobConfig& job_config) override;

    private:
    };
}
