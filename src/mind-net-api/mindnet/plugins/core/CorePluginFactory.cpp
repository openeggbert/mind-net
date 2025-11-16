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
#include "../../../../../include/mind-net-db-sqlite/mindnet/db/sqlite/queries/CleanupSQLiteQuery.hpp"
#include "../../../../../include/mind-net-db-sqlite/mindnet/db/sqlite/queries/CleanupHistoryOrphansSQLiteQuery.hpp"
#include "../../../../../include/mind-net-db-sqlite/mindnet/db/sqlite/queries/VacuumSQLiteQuery.hpp"

namespace mindnet::plugins::core
{
    const string CORE_PLUGIN_NAME = "core";

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
        plugin->register_trigger(std::make_shared<triggers::HistoryCommonTrigger>());
        plugin->register_job(std::make_shared<jobs::TestJob>());
        plugin->register_job(std::make_shared<jobs::CleanupJob>());
        plugin->register_job(std::make_shared<jobs::CleanupHistoryOrphansJob>());
        plugin->register_job(std::make_shared<jobs::VacuumJob>());
        plugin->register_query(std::make_shared<db::sqlite::queries::CleanupSQLiteQuery>());
        plugin->register_query(std::make_shared<db::sqlite::queries::CleanupHistoryOrphansSQLiteQuery>());
        plugin->register_query(std::make_shared<db::sqlite::queries::VacuumSQLiteQuery>());

        plugin->close_for_changes();
        return plugin;
    }
}
