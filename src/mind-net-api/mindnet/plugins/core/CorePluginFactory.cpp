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
#include "mindnet/plugins/core/CorePluginFactory.hpp"

#include "mindnet/plugins/core/migrations/CoreSQLiteMigrationScripts.hpp"
#include "mindnet/plugins/core/migrations/CorePostgreSQLMigrationScripts.hpp"
#include "mindnet/plugins/core/triggers/HistoryCommonTrigger.hpp"
#include "mindnet/plugins/core/jobs/TestJob.hpp"
#include "mindnet/plugins/core/jobs/CleanupJob.hpp"
#include "mindnet/plugins/core/jobs/CleanupHistoryOrphansJob.hpp"
#include "mindnet/plugins/core/jobs/VacuumJob.hpp"
#include "mindnet/plugins/core/validators/HistoryValidator.hpp"
#include "mindnet/plugins/core/validators/TeamMemberValidator.hpp"
#include "mindnet/plugins/core/validators/TeamValidator.hpp"
#include "mindnet/plugins/core/validators/UserValidator.hpp"
#include "mindnet/plugins/core/validators/ApiLogValidator.hpp"
#include "mindnet/plugins/core/validators/AccessTokenValidator.hpp"
#include "mindnet/plugins/core/validators/RefreshTokenValidator.hpp"
#include "mindnet/plugins/core/validators/LoginSessionValidator.hpp"
#include "mindnet/plugins/core/validators/AuthLogValidator.hpp"
#include "mindnet/plugins/core/validators/SuperAdminLogValidator.hpp"
#include "mindnet/plugins/core/validators/JobEntryValidator.hpp"
#include "mindnet/plugins/core/validators/JobRunValidator.hpp"
#include "mindnet/plugins/core/validators/ErrorValidator.hpp"
#include "../../../../../include/mind-net-db-sqlite/mindnet/db/sqlite/queries/core/CleanupSQLiteQuery.hpp"
#include "../../../../../include/mind-net-db-sqlite/mindnet/db/sqlite/queries/core/CleanupHistoryOrphansSQLiteQuery.hpp"
#include "../../../../../include/mind-net-db-sqlite/mindnet/db/sqlite/queries/core/VacuumSQLiteQuery.hpp"

namespace mindnet::plugins::core
{
    api::PluginPtr CorePluginFactory::create(std::shared_ptr<api::RepositoryFactory>& repository_factory) const
    {
        auto plugin = std::make_shared<api::Plugin>(
            CORE_PLUGIN_NAME,
            "core models"
        );
        REGISTER_MIGRATIONS(Core, SQLite)
        REGISTER_MIGRATIONS(Core, PostgreSQL)

        REGISTER_MODEL(user, User, USER)
        REGISTER_MODEL(team, Team, TEAM)
        REGISTER_MODEL(team_member, TeamMember, TEAM_MEMBER)
        REGISTER_MODEL(history, History, HISTORY)
        REGISTER_MODEL(api_log, ApiLog, API_LOG)
        REGISTER_MODEL(access_token, AccessToken, ACCESS_TOKEN)
        REGISTER_MODEL(refresh_token, RefreshToken, REFRESH_TOKEN)
        REGISTER_MODEL(login_session, LoginSession, LOGIN_SESSION)
        REGISTER_MODEL(auth_log, AuthLog, AUTH_LOG)
        REGISTER_MODEL(super_admin_log, SuperAdminLog, SUPER_ADMIN_LOG)
        REGISTER_MODEL(job_entry, JobEntry, JOB_ENTRY)
        REGISTER_MODEL(job_run, JobRun, JOB_RUN)
        REGISTER_MODEL(error, Error, ERROR)

        plugin->register_trigger(std::make_shared<triggers::HistoryCommonTrigger>());
        plugin->register_job(std::make_shared<jobs::TestJob>());
        plugin->register_job(std::make_shared<jobs::CleanupJob>());
        plugin->register_job(std::make_shared<jobs::CleanupHistoryOrphansJob>());
        plugin->register_job(std::make_shared<jobs::VacuumJob>());

        plugin->register_query(std::make_shared<db::sqlite::queries::core::CleanupSQLiteQuery>());
        plugin->register_query(std::make_shared<db::sqlite::queries::core::CleanupHistoryOrphansSQLiteQuery>());
        plugin->register_query(std::make_shared<db::sqlite::queries::core::VacuumSQLiteQuery>());

        plugin->close_for_changes();
        return plugin;
    }
}