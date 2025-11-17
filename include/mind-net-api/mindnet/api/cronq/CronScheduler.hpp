#pragma once
#include <chrono>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <vector>
#include <functional>
#include <queue>
#include <future>

#include "cron_quartz.hpp"
#include "mindnet/api/AbstractTriggerJob.hpp"
#include "mindnet/api/Job.hpp"

namespace mindnet::api::cronq
{
    class CronScheduler : public AbstractTriggerJob
    {
    public:
        CronScheduler();
        ~CronScheduler();

        void start();
        void stop();
        void add_job(JobPtr& job);

    private:
        struct ScheduledJobEntry
        {
            JobPtr job;
            cronq::CronExpr cron;
            std::chrono::system_clock::time_point next_run;
            i64 job_id;
            std::string job_name;
            bool enabled = true;
            std::chrono::system_clock::time_point last_started_at;
            bool running{false};
            unixtime last_enabled_check = 0;
            JobConfig job_config = JobConfig("");

            ScheduledJobEntry(const ScheduledJobEntry&) = default;

            ScheduledJobEntry& operator=(const ScheduledJobEntry&) = delete;

            ScheduledJobEntry(ScheduledJobEntry&&) noexcept = default;
            ScheduledJobEntry& operator=(ScheduledJobEntry&&) noexcept = default;

            ScheduledJobEntry(
                JobPtr job_,
                const cronq::CronExpr& cron_,
                std::chrono::system_clock::time_point last_started_at_,
                i64 job_id_,
                const std::string& job_name_,
                bool enabled_,
                std::chrono::system_clock::time_point next_run_,
                i64 last_enabled_check_,
                JobConfig cfg_
            )
                : job(std::move(job_))
                  , cron(cron_)
                  , next_run(next_run_)
                  , job_id(job_id_)
                  , job_name(job_name_)
                  , enabled(enabled_)
                  , last_started_at(last_started_at_)
                  , running(false)
                  , last_enabled_check(last_enabled_check_)
                  , job_config(std::move(cfg_))
            {
            }
        };


        std::vector<JobPtr> all_job_ptrs;
        std::thread scheduler_thread_;
        std::atomic<bool> running_{false};

        std::mutex mtx_;
        std::condition_variable cv_;

        std::vector<ScheduledJobEntry> jobs_;

        // --- threadpool (simple fixed pool 4 threads) ---
        std::vector<std::thread> workers_;
        std::queue<std::function<void()>> task_queue_;
        std::mutex queue_mtx_;
        std::condition_variable queue_cv_;
        std::atomic<bool> pool_running_{false};

        // ---
        void scheduler_loop();
        void load_jobs_from_db();
        bool load_enabled_from_db(i64 job_id);
        bool load_enabled_and_configuration(i64 job_id, bool& enabled_out, std::string& cfg_out);
        void compute_initial_next_runs();
        void sleep_until_next_job();
        ScheduledJobEntry* find_next_job();

        // threadpool
        void start_threadpool(int threads = 4);
        void stop_threadpool();
        void enqueue_task(std::function<void()> fn);

        void run_job(ScheduledJobEntry& entry);

        // SQLite helpers
        i64 insert_job_run(const i64 job_id,
                           unixtime start_time);

        void update_job_run(const i64 run_id,
                            unixtime finish_time,
                            bool success,
                            const std::string& message);

        void update_next_run_in_db(i64 job_id,
                                   unixtime tp);
    };

    typedef std::shared_ptr<CronScheduler> CronSchedulerPtr;
}
