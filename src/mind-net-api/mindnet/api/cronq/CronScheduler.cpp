#include "mindnet/api/cronq/CronScheduler.hpp"
#include <iomanip>

#include "mindnet/api/AccessTokenContext.hpp"
#include "mindnet/essential/Global.hpp"
#include "mindnet/orm/QueryParams.hpp"
#include "mindnet/plugins/core/models/JobEntry.hpp"
#include "mindnet/plugins/core/models/JobRun.hpp"
#include "mindnet/plugins/core/validators/JobEntryValidator.hpp"
#include "mindnet/util/Utils.hpp"

namespace mindnet::api
{
    using namespace std::chrono;

    CronScheduler::CronScheduler()
        : AbstractTriggerJob("CronScheduler", "CronScheduler")
    {
    }

    CronScheduler::~CronScheduler()
    {
        stop();
    }

    void CronScheduler::start()
    {
        if (all_job_ptrs.empty()) return;
        essential::info << "Starting CronScheduler" << essential::commit;
        running_ = true;
        pool_running_ = true;

        load_jobs_from_db(); // loads jobs from DB including cron expressions
        compute_initial_next_runs(); // compute next_run for all jobs

        start_threadpool(4); // 4 worker threads

        scheduler_thread_ = std::thread(&CronScheduler::scheduler_loop, this);
    }

    void CronScheduler::stop()
    {
        if (all_job_ptrs.empty()) return;
        running_ = false;
        pool_running_ = false;

        cv_.notify_all();
        queue_cv_.notify_all();

        if (scheduler_thread_.joinable())
            scheduler_thread_.join();

        stop_threadpool();
    }

    void CronScheduler::add_job(JobPtr& job)
    {
        all_job_ptrs.push_back(job);
    }

    void CronScheduler::load_jobs_from_db()
    {
        api::AccessTokenContext ctx(0, "system", 403);
        orm::QueryParams params;
        auto jobs_in_db = run_list(
            plugins::core::models::JOB_ENTRY_DEFINITION,
            ctx,
            params,
            0
        );
        if (jobs_in_db.second.ko())
        {
            throw std::runtime_error(std::string("Loading jobs from database failed: ") + jobs_in_db.second.error);
        }
        std::map<string, plugins::core::models::JobEntry> jobs_in_db_map;
        for (auto& e : jobs_in_db.first)
        {
            plugins::core::models::JobEntry job_entry;
            job_entry.from_values(e);
            jobs_in_db_map.insert({job_entry.name, job_entry});
        }

        for (auto& job_ptr : all_job_ptrs)
        {
            bool enabled = true;
            i64 id = 0;

            // Synchronize software and database
            if (jobs_in_db_map.contains(job_ptr->get_name()))
            {
                plugins::core::models::JobEntry& job_entry = jobs_in_db_map[job_ptr->get_name()];
                id = job_entry.get_id();
                job_entry.description = job_ptr->get_description();
                job_entry.plugin_name = job_ptr->get_plugin_name();
                job_entry.cron_expression = job_ptr->get_cron_expression();
                job_entry.run_once_when_missed = job_ptr->get_run_once_when_missed();
                enabled = job_entry.enabled;
                auto job_entry_values = job_entry.to_values();
                job_entry_values[1] = util::Utils::current_unix_timestamp_ms();
                job_entry_values[2] = util::Utils::current_unix_timestamp_ms();

                auto update_job_entry = run_update(
                    plugins::core::models::JOB_ENTRY_DEFINITION,
                    ctx,
                    job_entry.get_id(),
                    job_entry_values,
                    0
                );
                if (update_job_entry.ko())
                {
                    throw std::runtime_error(std::string("Updating job failed: ") + update_job_entry.error);
                }
            }
            else
            {
                plugins::core::models::JobEntry job_entry;
                job_entry.name = job_ptr->get_name();
                job_entry.description = job_ptr->get_description();
                job_entry.plugin_name = job_ptr->get_plugin_name();
                job_entry.cron_expression = job_ptr->get_cron_expression();
                job_entry.run_once_when_missed = job_ptr->get_run_once_when_missed();
                job_entry.enabled = enabled;
                job_entry.last_run = 0;
                job_entry.next_run = 0;
                auto job_entry_values = job_entry.to_values();
                job_entry_values[2] = util::Utils::current_unix_timestamp_ms();

                auto create_job_entry = run_create(
                    plugins::core::models::JOB_ENTRY_DEFINITION,
                    ctx,
                    job_entry_values,
                    0
                );
                if (create_job_entry.second.ko())
                {
                    throw std::runtime_error(std::string("Creating job failed: ") + create_job_entry.second.error);
                }
                else
                {
                    jobs_in_db_map[job_entry.name] = job_entry;
                    id = create_job_entry.first;
                }
            }
            cronq::CronExpr expr = cronq::parse_cron_quartz(job_ptr->get_cron_expression());

            jobs_.push_back({
                job_ptr,
                expr,
                system_clock::now(), // placeholder
                id,
                job_ptr->get_name(),
                enabled
            });
        }
    }

    unixtime system_clock_to_unixtime(std::chrono::system_clock::time_point tp)
    {
        long long unix_milliseconds =
            std::chrono::duration_cast<std::chrono::milliseconds>(
                tp.time_since_epoch()
            ).count();
        return unix_milliseconds;
    }

    void CronScheduler::compute_initial_next_runs()
    {
        auto now = system_clock::now();

        for (auto& j : jobs_)
        {
            // ----------------------------------------------
            // RUN_AT_START: run exactly once at startup
            // ----------------------------------------------
            if (j.cron.run_at_start)
            {
                auto* job_ptr = &j;
                enqueue_task([this, job_ptr] {
                    run_job(*job_ptr);
                });

                j.next_run = std::chrono::system_clock::time_point::max();

                continue;
            }

            // 1) Find the next scheduled job run (future)
            auto next_scheduled = j.cron.next_after(now);

            // 2) Get the last scheduled run BEFORE "now"
            auto prev_scheduled = j.cron.previous_before(now);
            // (the previous_before method will be added below - simple)

            // 3) Load last_run from database
            // (we have it in JobEntry)
            i64 last_run_ts = 0;
            {
                api::AccessTokenContext ctx(0, "system", 403);
                auto read_job_entry = run_read(
                    plugins::core::models::JOB_ENTRY_DEFINITION,
                    ctx,
                    j.job_id,
                    0
                );
                plugins::core::models::JobEntry job_entry;
                job_entry.from_values(read_job_entry.first);
                last_run_ts = job_entry.last_run;
            }

            bool missed =
                j.job->get_run_once_when_missed() &&
                prev_scheduled > std::chrono::system_clock::time_point(std::chrono::milliseconds(last_run_ts));

            if (missed)
            {
                // 🚀 RUN IMMEDIATELY (WITHOUT WAITING)
                enqueue_task([this, job_ptr = &j]
                {
                    run_job(*job_ptr);
                });

                // set next_run to the next scheduled future run
                j.next_run = next_scheduled;
            }
            else
            {
                // nothing was missed -> regular scheduling
                j.next_run = next_scheduled;
            }

            update_next_run_in_db(j.job_id, system_clock_to_unixtime(j.next_run));
        }
    }

    void CronScheduler::scheduler_loop()
    {
        while (running_)
        {
            auto* next = find_next_job();
            if (!next)
            {
                std::this_thread::sleep_for(std::chrono::seconds(1L));
                continue;
            }

            auto scheduled = next->next_run;
            auto now = system_clock::now();

            // Wait until scheduled time
            if (scheduled > now)
            {
                std::unique_lock lk(mtx_);
                cv_.wait_until(lk, scheduled, [this] { return !running_; });
            }

            if (!running_) return;

            // ------------------------------------------
            // 🚫 Prevent parallel execution of same job
            // ------------------------------------------
            if (next->running)
            {
                // Job is still executing -> skip this occurrence
                // Plan next run
                next->next_run = next->cron.next_after(scheduled);
                update_next_run_in_db(next->job_id, system_clock_to_unixtime(next->next_run));
                continue;
            }

            // ------------------------------------------
            // Mark as running
            // ------------------------------------------
            next->running = true;

            // ------------------------------------------
            // Execute job (in thread pool)
            // ------------------------------------------
            enqueue_task([this, next]
            {
                run_job(*next);

                // clear running flag AFTER job fully finished
                next->running = false;
            });

            // ------------------------------------------
            // Plan next run
            // ------------------------------------------
            next->next_run = next->cron.next_after(scheduled);
            update_next_run_in_db(next->job_id, system_clock_to_unixtime(next->next_run));
        }
    }

    CronScheduler::ScheduledJobEntry* CronScheduler::find_next_job()
    {
        if (jobs_.empty()) return nullptr;

        ScheduledJobEntry* best = &jobs_[0];
        for (auto& j : jobs_)
        {
            if (j.next_run < best->next_run)
                best = &j;
        }
        return best;
    }

    void CronScheduler::start_threadpool(int threads)
    {
        for (int i = 0; i < threads; i++)
        {
            workers_.emplace_back([this]
            {
                while (pool_running_)
                {
                    std::function<void()> task;

                    {
                        std::unique_lock lk(queue_mtx_);
                        queue_cv_.wait(lk, [this]
                        {
                            return !pool_running_ || !task_queue_.empty();
                        });
                        if (!pool_running_) return;
                        task = std::move(task_queue_.front());
                        task_queue_.pop();
                    }

                    try
                    {
                        task();
                    }
                    catch (...)
                    {
                        // Log exception somewhere
                    }
                }
            });
        }
    }

    void CronScheduler::stop_threadpool()
    {
        queue_cv_.notify_all();
        for (auto& w : workers_)
        {
            if (w.joinable())
                w.join();
        }
    }

    void CronScheduler::enqueue_task(std::function<void()> fn)
    {
        {
            std::unique_lock lk(queue_mtx_);
            task_queue_.push(std::move(fn));
        }
        queue_cv_.notify_one();
    }

    void CronScheduler::run_job(ScheduledJobEntry& entry)
    {
        auto start = system_clock::now();

        // insert job_run row
        const i64 job_id = entry.job_id;
        unixtime start_unixtime = system_clock_to_unixtime(start);
        i64 run_id = 0;
        try
        {
            run_id = insert_job_run(job_id, start_unixtime);
        } catch (const std::exception& ex)
        {
            essential::err << "Job failed, because insert_job_run failed: " << ex.what() << essential::commit;
        }

        bool success = true;
        std::string message = "OK";

        try
        {
            entry.last_started_at = std::chrono::system_clock::now();
            entry.job->run(); // *** actual job code ***
        }
        catch (const std::exception& ex)
        {
            success = false;
            message = ex.what();
        }

        auto finish = system_clock::now();

        update_job_run(run_id, system_clock_to_unixtime(finish), success, message);
    }

    i64 CronScheduler::insert_job_run(
        const i64 job_id,
        unixtime start_time
    )
    {
        api::AccessTokenContext ctx(0, "system", 403);

        plugins::core::models::JobRun job_run;
        job_run.jon_entry_id = job_id;
        job_run.started_at = start_time;
        job_run.finished_at = 0;
        job_run.success = false;
        job_run.message = "Started";
        auto job_run_values = job_run.to_values();
        job_run_values[1] = util::Utils::current_unix_timestamp_ms();
        job_run_values[2] = util::Utils::current_unix_timestamp_ms();

        auto create_job_run = run_create(
            plugins::core::models::JOB_RUN_DEFINITION,
            ctx,
            job_run_values,
            0
        );
        if (create_job_run.second.ko())
        {
            throw std::runtime_error("Cannot create job run: " + create_job_run.second.error);
        }

        return create_job_run.first;
    }

    void CronScheduler::update_job_run(
        const i64 run_id,
        unixtime finish_time,
        bool success,
        const std::string& message)
    {
        api::AccessTokenContext ctx(0, "system", 403);

        auto read_job_run = run_read(
            plugins::core::models::JOB_RUN_DEFINITION,
            ctx,
            run_id,
            0
        );
        if (read_job_run.second.ko())
        {
            throw std::runtime_error("Cannot read job run: " + read_job_run.second.error);
        }

        plugins::core::models::JobRun job_run;
        job_run.from_values(read_job_run.first);
        job_run.finished_at = finish_time;
        job_run.success = success;
        job_run.message = message;
        auto job_run_values = job_run.to_values();
        job_run_values[2] = util::Utils::current_unix_timestamp_ms();

        auto update_job_run_result = run_update(
            plugins::core::models::JOB_RUN_DEFINITION,
            ctx,
            run_id,
            job_run_values,
            0
        );
        if (update_job_run_result.ko())
        {
            throw std::runtime_error("Cannot update job run: " + update_job_run_result.error);
        }





        auto read_job_run2 = run_read(
            plugins::core::models::JOB_RUN_DEFINITION,
            ctx,
            run_id,
            0
        );

        plugins::core::models::JobRun jr2;
        jr2.from_values(read_job_run2.first);

        // Load job entry
        auto read_job_entry = run_read(
            plugins::core::models::JOB_ENTRY_DEFINITION,
            ctx,
            jr2.jon_entry_id,
            0
        );

        plugins::core::models::JobEntry entry;
        entry.from_values(read_job_entry.first);

        entry.last_run = finish_time;
        auto vals = entry.to_values();
        vals[2] = util::Utils::current_unix_timestamp_ms();

        auto update_job_entry = run_update(
            plugins::core::models::JOB_ENTRY_DEFINITION,
            ctx,
            entry.get_id(),
            vals,
            0
        );
        if (update_job_entry.ko())
            throw std::runtime_error("Cannot update JobEntry.last_run");
    }

    void CronScheduler::update_next_run_in_db(
        const i64 job_id,
        unixtime next_run)
    {
        api::AccessTokenContext ctx(0, "system", 403);

        auto read_job_entry = run_read(
            plugins::core::models::JOB_ENTRY_DEFINITION,
            ctx,
            job_id,
            0
        );
        if (read_job_entry.second.ko())
        {
            throw std::runtime_error("Cannot read job entry: " + read_job_entry.second.error);
        }

        plugins::core::models::JobEntry job_entry;
        job_entry.from_values(read_job_entry.first);
        job_entry.next_run = next_run;
        auto job_entry_values = job_entry.to_values();
        job_entry_values[2] = util::Utils::current_unix_timestamp_ms();

        auto update_job_entry = run_update(
            plugins::core::models::JOB_ENTRY_DEFINITION,
            ctx,
            job_entry.get_id(),
            job_entry_values,
            0
        );
        if (update_job_entry.ko())
        {
            throw std::runtime_error("Cannot update job entry: " + update_job_entry.error);
        }
    }
}
