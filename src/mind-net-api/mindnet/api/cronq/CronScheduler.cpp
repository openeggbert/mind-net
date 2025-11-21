#include "mindnet/api/cronq/CronScheduler.hpp"
#include <iomanip>

#include "mindnet/api/AccessTokenContext.hpp"
#include "mindnet/essential/Global.hpp"
#include "mindnet/orm/QueryParams.hpp"
#include "mindnet/plugins/core/models/JobEntry.hpp"
#include "mindnet/plugins/core/models/JobRun.hpp"
#include "mindnet/plugins/core/validators/JobEntryValidator.hpp"
#include "mindnet/util/Utils.hpp"
#include "mindnet/api/Service.hpp"

namespace mindnet::api::cronq
{
    using namespace std::chrono;

    static constexpr long DO_REFRESH_EACH_X_SECONDS = 300L;

    CronScheduler::CronScheduler()
        : AbstractTriggerJob("CronScheduler", "CronScheduler")
    {
        essential::info
    << "[CRON] CronScheduler::Ctor this=" << (void*)this
    << " thread_id=" << std::this_thread::get_id()
    << essential::commit;
    }

    CronScheduler::~CronScheduler()
    {
        essential::info
    << "[CRON] CronScheduler::Dtor this=" << (void*)this
    << " thread_id=" << std::this_thread::get_id()
    << essential::commit;
        stop();
    }

    void CronScheduler::start()
    {
        essential::info
    << "[CRON] CronScheduler::start this=" << (void*)this
    << " running_=" << running_.load()
    << " thread_id=" << std::this_thread::get_id()
    << essential::commit;
        if (all_job_ptrs.empty())
        {
            essential::warn << "CronScheduler: no jobs registered, but starting anyway" << essential::commit;
        }
        essential::info << "Starting CronScheduler" << essential::commit;
        running_ = true;
        pool_running_ = true;

        load_jobs_from_db(); // loads jobs from DB including cron expressions
        compute_initial_next_runs(); // compute next_run for all jobs

        essential::err << "[CRON-POOL] starting threadpool NOW" << essential::commit;
        start_threadpool(4); // 4 worker threads

        scheduler_thread_ = std::thread(&CronScheduler::scheduler_loop, this);
    }

    void CronScheduler::stop()
    {
        essential::warn
    << "[CRON] CronScheduler::stop this=" << (void*)this
    << " running_=" << running_.load()
    << " pool_running_=" << pool_running_.load()
    << " thread_id=" << std::this_thread::get_id()
    << essential::commit;
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
            bool enabled = job_ptr->get_enabled_by_default();
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
            essential::info
                << "[CRON-LOAD] job=" << job_ptr->get_name()
                << " raw='" << job_ptr->get_cron_expression() << "'"
                << essential::commit;


            std::string cfg_text = jobs_in_db_map[job_ptr->get_name()].configuration;
            JobConfig cfg(cfg_text);

            jobs_.emplace_back(
                job_ptr,
                expr,
                system_clock::now(),
                id,
                job_ptr->get_name(),
                enabled,
                std::chrono::system_clock::time_point{},
                0,
                cfg
            );
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

    bool CronScheduler::load_enabled_from_db(i64 job_id)
    {
        api::AccessTokenContext ctx(0, "system", 403);

        auto read_job_entry = run_read(
            plugins::core::models::JOB_ENTRY_DEFINITION,
            ctx,
            job_id,
            0
        );

        if (read_job_entry.second.ko())
            return false;

        plugins::core::models::JobEntry entry;
        entry.from_values(read_job_entry.first);

        return entry.enabled;
    }

    bool CronScheduler::load_enabled_and_configuration(i64 job_id, bool& enabled_out, std::string& cfg_out)
    {
        api::AccessTokenContext ctx(0, "system", 403);

        auto read_job_entry = run_read(
            plugins::core::models::JOB_ENTRY_DEFINITION,
            ctx,
            job_id,
            0
        );

        if (read_job_entry.second.ko())
            return false;

        plugins::core::models::JobEntry entry;
        entry.from_values(read_job_entry.first);

        enabled_out = entry.enabled;
        cfg_out = entry.configuration;
        return true;
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
                enqueue_task([this, job_ptr]
                {
                    run_job(*job_ptr);
                });

                j.next_run = std::chrono::system_clock::time_point::max();

                continue;
            }

            // 1) Find the next scheduled job run (future)
            essential::info
                << "[CRON-COMPUTE] job=" << j.job_name
                << " cron='" << j.job->get_cron_expression() << "'"
                << essential::commit;

            auto next_scheduled = j.cron.next_after(now);
            essential::info
                << "[CRON-COMPUTE] next_after="
                << util::Utils::unixtime_to_string(system_clock_to_unixtime(next_scheduled))
                << essential::commit;


            if (next_scheduled <= now) {
                essential::info
                    << "[CRON-COMPUTE-ERROR] next_after returned PAST DATE!"
                    << " cron=" << j.job->get_cron_expression()
                    << essential::commit;
            }



            // compute initial next_run — Quartz previous_before() removed (buggy)
            j.next_run = next_scheduled;

            update_next_run_in_db(j.job_id, system_clock_to_unixtime(j.next_run));
        }
    }

    void CronScheduler::scheduler_loop()
    {
        try
        {
            essential::info << "[CRON-LOOP] THREAD ENTERED" << essential::commit;
            auto last_refresh = std::chrono::steady_clock::now();
            while (running_)
            {
                std::this_thread::sleep_for(std::chrono::milliseconds(20L));
                essential::info << "[CRON-LOOP] top of while" << essential::commit;
                // 1) LOAD ENABLED + CONFIG ONLY ONCE PER MINUTE
                auto now_sc = std::chrono::steady_clock::now();
                bool do_refresh = (now_sc - last_refresh) >= std::chrono::seconds(DO_REFRESH_EACH_X_SECONDS);

                if (do_refresh)
                {
                    last_refresh = now_sc;

                    auto now_ms = util::Utils::current_unix_timestamp_ms();

                    for (auto& j : jobs_)
                    {
                        bool new_enabled;
                        std::string new_cfg;

                        if (!load_enabled_and_configuration(j.job_id, new_enabled, new_cfg))
                        {
                            essential::err
                                << "[CRON-CHECK] FAILED load_enabled_and_configuration job_id="
                                << j.job_id
                                << essential::commit;
                            continue;
                        }

                        if (new_enabled != j.enabled)
                        {
                            essential::info
                                << "[CRON-CHECK] job=" << j.job_name
                                << " state change: local_enabled=" << j.enabled
                                << " db_enabled=" << new_enabled
                                << essential::commit;
                        }

                        j.enabled = new_enabled;

                        // Disable → next_run=max()
                        if (!j.enabled)
                        {
                            j.running = false;
                            j.next_run = std::chrono::system_clock::time_point::max();
                            update_next_run_in_db(j.job_id, 0);
                            continue;
                        }

                        // Enable → next_run==max() → recompute
                        if (j.next_run == std::chrono::system_clock::time_point::max())
                        {
                            j.running = false;
                            j.next_run = j.cron.next_after(std::chrono::system_clock::now());
                            update_next_run_in_db(j.job_id, system_clock_to_unixtime(j.next_run));
                            continue;
                        }


//avoid running jobs, which should not be running
                        auto nowtp = std::chrono::system_clock::now();

                        // Case 1: enabled after downtime, compute proper next
                        auto next_after = j.cron.next_after(nowtp);

                        // If run_once_when_missed and the scheduled time was in the past → run NOW (once)
                        // detect missed run based on stored next_run (not recomputed)
                        if (j.job->get_run_once_when_missed() && j.next_run <= std::chrono::system_clock::now())
                        {
                            // run job once
                            auto* job_ptr = &j;
                            enqueue_task([this, job_ptr]{ run_job(*job_ptr); });

                            // compute next future run
                            j.next_run = j.cron.next_after(std::chrono::system_clock::now());
                            update_next_run_in_db(j.job_id, system_clock_to_unixtime(j.next_run));
                            continue;
                        }

                        // // normal case
                        // j.next_run = next_after;
                        // update_next_run_in_db(j.job_id, system_clock_to_unixtime(j.next_run));
                        // continue;





                        // Configuration change
                        if (util::Utils::compute_sha256(new_cfg) != j.job_config.get_sha256())
                        {
                            j.job_config = JobConfig(new_cfg);
                            essential::info
                                << "[CRON-CONFIG] job=" << j.job_name
                                << " configuration reloaded"
                                << essential::commit;
                        }
                    }
                }



                essential::info
                    << "[CRON-LOOP] listing jobs:"
                    << essential::commit;

                for (auto& j : jobs_)
                {
                    essential::info
                        << "[CRON-LOOP] job=" << j.job_name
                        << " enabled=" << j.enabled
                        << " next_run=" << util::Utils::unixtime_to_string(system_clock_to_unixtime(j.next_run))
                        << essential::commit;
                }

                //
                // --- GLOBAL BLOCKER: maintenance / shutdown / restart ---
                //
                if (essential::g_configuration.access_mode == essential::AccessMode::MaintenanceMode ||
                    service_ptr->is_shutdown_scheduled() ||
                    service_ptr->is_restart_scheduled())
                {
                    essential::info
                        << "[CRON-BLOCK] Scheduler paused for 1 minute due to system state (maintenance/shutdown/restart)"
                        << essential::commit;

                    // Sleep with periodic wake for shutdown safety
                    for (int i = 0; i < 60 && running_; i++)
                    {
                        std::unique_lock lk(mtx_);
                        cv_.wait_for(lk, std::chrono::seconds(1L), [this] { return !running_; });
                    }

                    // Go to next iteration - DO NOT evaluate jobs now
                    continue;
                }


                // 2) FIND NEXT JOB TO RUN
                auto* next = find_next_job();
                if (next == nullptr)
                {
                    essential::info
                        << "[CRON-LOOP] next=nullptr"
                        << essential::commit;
                }
                else
                {
                    essential::info
                        << "[CRON-LOOP] selected=" << next->job_name
                        << " next_run=" << util::Utils::unixtime_to_string(system_clock_to_unixtime(next->next_run))
                        << essential::commit;
                }

                // --- NEW unified sleep logic (no busy loop, max 60s sleep) ---

                auto wake_at = next ? next->next_run : std::chrono::system_clock::time_point::max();

                if (wake_at > std::chrono::system_clock::now())
                {
                    std::unique_lock lk(mtx_);
                    essential::err << "[SCHED] WAIT start" << essential::commit;
                    cv_.wait_until(lk, wake_at, [this]{ return !running_; });
                    essential::err << "[SCHED] WAIT done" << essential::commit;

                }

                if (!running_)
                    return;

                if (!next || !next->enabled || next->next_run == std::chrono::system_clock::time_point::max())
                {
                    essential::err << "[SCHED] CONTINUE #1: next invalid" << essential::commit;
                    continue;
                }




                if (!running_) return;

                // 4) PREVENT PARALLEL EXECUTION OF THE SAME JOB
                if (next->running)
                {
                    essential::err << "[SCHED] CONTINUE #3: JOB STILL RUNNING" << essential::commit;
                    std::this_thread::sleep_for(10ms);
                    std::this_thread::sleep_for(std::chrono::seconds(1L));
                    continue;
                }

                next->running = true;

                // 5) LAUNCH JOB IN THREADPOOL
                enqueue_task([this, next]
                {
                    essential::err << "[JOB] START " << next->job_name << essential::commit;
                    run_job(*next);
                    essential::err << "[JOB] END " << next->job_name << essential::commit;
                    next->running = false;
                });

                // 6) SCHEDULE NEXT RUN – based on the OLD next_run
                auto baseline = next->next_run + std::chrono::seconds(1L);
                auto next_scheduled = next->cron.next_after(baseline);

                next->next_run = next_scheduled;
                update_next_run_in_db(next->job_id, system_clock_to_unixtime(next_scheduled));


                std::this_thread::sleep_for(std::chrono::milliseconds(10L));
            }

            essential::warn
                << "[CRON-LOOP] thread EXITING normal while(running_) loop this=" << (void*)this
                << essential::commit;
        }
        catch (const std::exception& ex)
        {
            essential::err << "[CRON-LOOP] THREAD EXCEPTION: " << ex.what() << essential::commit;
        }
        catch (...)
        {
            essential::err << "[CRON-LOOP] THREAD EXCEPTION UNKNOWN" << essential::commit;
        }
    }

    CronScheduler::ScheduledJobEntry* CronScheduler::find_next_job()
    {
        ScheduledJobEntry* best = nullptr;

        for (auto& j : jobs_)
        {
            if (!j.enabled)
                continue;

            if (j.next_run == std::chrono::system_clock::time_point::max())
                continue;

            if (!best || j.next_run < best->next_run)
                best = &j;
        }

        return best;
    }


    void CronScheduler::start_threadpool(int threads)
    {
        essential::err << "[CRON-POOL] creating " << threads << " workers" << essential::commit;
        for (int i = 0; i < threads; i++)
        {
            workers_.emplace_back([this]
            {
                essential::err << "[CRON-POOL] WORKER STARTED id="
               << std::this_thread::get_id()
               << essential::commit;

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
        }
        catch (const std::exception& ex)
        {
            essential::err << "Job failed, because insert_job_run failed: " << ex.what() << essential::commit;
        }

        bool success = true;
        std::string message = "OK";

        try
        {
            entry.last_started_at = std::chrono::system_clock::now();
            essential::err << "[JOB] ENTER " << entry.job_name << essential::commit;
            std::string result = entry.job->run(entry.job_config); // *** actual job code ***
            essential::err << "[JOB] EXIT  " << entry.job_name << essential::commit;
            if (!result.empty() && result != "OK")
            {
                success = false;
                message = result;
            }
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
        job_run.job_entry_id = job_id;
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
            jr2.job_entry_id,
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

