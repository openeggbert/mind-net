#include "mindnet/plugins/core/CorePluginFactory.hpp"

#include "mindnet/plugins/core/migrations/CoreSQLiteMigrationScripts.hpp"
#include "mindnet/plugins/core/migrations/CorePostgreSQLMigrationScripts.hpp"
#include "mindnet/plugins/core/triggers/HistoryCommonTrigger.hpp"
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
        plugin->register_trigger(std::make_shared<triggers::HistoryCommonTrigger>());

        plugin->close_for_changes();
        return plugin;
    }
}
