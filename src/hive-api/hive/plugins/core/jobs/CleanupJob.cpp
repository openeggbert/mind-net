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

#include "hive/plugins/core/jobs/CleanupJob.hpp"

#include "hive/essential/Global.hpp"
#include "hive/util/Utils.hpp"

namespace hive::plugins::core::jobs
{
    using_loggers()

    CleanupJob::CleanupJob()
        : Job(
            "CleanupJob",
            "CleanupJob",
            "@daily",
            true,
            true
        )
    {
    }

    std::string CleanupJob::run(api::cronq::JobConfig& job_config)
    {
        essential::info << "CleanupJob TestJob (" << util::Utils::current_unixtime_to_string() << essential::commit;

        constexpr const static int DEFAULT_DAYS = 30;
        auto api_log_threshold_in_days = job_config.get_int_or_default("api_log_threshold_in_days", DEFAULT_DAYS);
        auto history_read_threshold_in_days = job_config.get_int_or_default("history_read_threshold_in_days", DEFAULT_DAYS);
        auto history_list_threshold_in_days = job_config.get_int_or_default("history_list_threshold_in_days", DEFAULT_DAYS);
        auto login_session_threshold_in_days = job_config.get_int_or_default("login_session_threshold_in_days", DEFAULT_DAYS);
        auto access_token_threshold_in_days = job_config.get_int_or_default("access_token_threshold_in_days", DEFAULT_DAYS);

        nlohmann::json req;
        req["api_log_threshold_in_days"] = cast64(api_log_threshold_in_days.first);
        req["history_read_threshold_in_days"] = cast64(history_read_threshold_in_days.first);
        req["history_list_threshold_in_days"] = cast64(history_list_threshold_in_days.first);
        req["login_session_threshold_in_days"] = cast64(login_session_threshold_in_days.first);
        req["access_token_threshold_in_days"] = cast64(access_token_threshold_in_days.first);

        nlohmann::json result = call_query("Cleanup", req);

        return result.contains("error") ? result["error"] : "";
    }

    using_loggers()
}