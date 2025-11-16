//
// Created by robertvokac on 9/7/25.
//
#ifndef TESTJOB_HPP
#define TESTJOB_HPP

#include "mindnet/api/Job.hpp"
#include "mindnet/essential/Global.hpp"

namespace mindnet::plugins::core::jobs
{
    class TestJob : public api::Job
    {
    public:
        TestJob();

        ~TestJob() = default;

        std::string run(api::cronq::JobConfig& job_config) override;

    private:
    };
}
#endif // TESTJOB_HPP
