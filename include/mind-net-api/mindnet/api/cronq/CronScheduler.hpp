/*
 * MIT License
 * Copyright (c) 2025 Robert Vokac
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */
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
            identification job_id;
            std::string job_name;
            bool enabled = true;
            std::chrono::system_clock::time_point last_started_at;
            bool running{false};
            unixtime last_enabled_check = 0;
            JobConfig job_config = JobConfig("");

            // --- COPY CONSTRUCTOR (OK) ---
            ScheduledJobEntry(const ScheduledJobEntry&) = default;

            // --- COPY ASSIGNMENT (FORBIDDEN!) ---
            ScheduledJobEntry& operator=(const ScheduledJobEntry&) = delete;

            // --- MOVE CONSTRUCTOR / ASSIGNMENT ---
            ScheduledJobEntry(ScheduledJobEntry&&) noexcept = default;
            ScheduledJobEntry& operator=(ScheduledJobEntry&&) noexcept = default;

            ScheduledJobEntry(
                JobPtr job_,
                const cronq::CronExpr& cron_,
                std::chrono::system_clock::time_point last_started_at_,
                identification job_id_,
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
        bool load_enabled_from_db(identification job_id);
        bool load_enabled_and_configuration(identification job_id, bool& enabled_out, std::string& cfg_out);
        void compute_initial_next_runs();
        void sleep_until_next_job();
        ScheduledJobEntry* find_next_job();

        // threadpool
        void start_threadpool(int threads = 4);
        void stop_threadpool();
        void enqueue_task(std::function<void()> fn);

        void run_job(ScheduledJobEntry& entry);

        // SQLite helpers
        identification insert_job_run(const identification job_id,
                           unixtime start_time);

        void update_job_run(const identification run_id,
                            unixtime finish_time,
                            bool success,
                            const std::string& message);

        void update_next_run_in_db(identification job_id,
                                   unixtime tp);
    };

    typedef std::shared_ptr<CronScheduler> CronSchedulerPtr;
}

