#include "mindnet/other/plugins/core/CorePluginFactory.h"

#include "mindnet/other/plugins/core/migrations/CoreSQLiteMigrationScripts.h"
#include "mindnet/other/plugins/core/triggers/HistoryCommonTrigger.h"
#include "mindnet/other/plugins/core/validators/HistoryValidator.h"
#include "mindnet/other/plugins/core/validators/TeamMemberValidator.h"
#include "mindnet/other/plugins/core/validators/TeamValidator.h"
#include "mindnet/other/plugins/core/validators/UserValidator.h"
#include "mindnet/other/plugins/core/validators/ApiLogValidator.h"
#include "mindnet/other/plugins/core/validators/AccessTokenValidator.h"
#include "mindnet/other/plugins/core/validators/RefreshTokenValidator.h"
#include "mindnet/other/plugins/core/validators/LoginSessionValidator.h"

namespace mindnet::plugins::core
{
    const string CORE_PLUGIN_NAME = "core";
    api::PluginPtr CorePluginFactory::create() const
    {
        auto plugin = std::make_shared<api::Plugin>(
            CORE_PLUGIN_NAME,
            "core models"
        );
        REGISTER_MIGRATIONS(Core, SQLite)

        REGISTER_MODEL(user, User, USER)
        REGISTER_MODEL(team, Team, TEAM)
        REGISTER_MODEL(team_member, TeamMember, TEAM_MEMBER)
        REGISTER_MODEL(history, History, HISTORY)
        REGISTER_MODEL(api_log, ApiLog, API_LOG)
        REGISTER_MODEL(access_token, AccessToken, ACCESS_TOKEN)
        REGISTER_MODEL(refresh_token, RefreshToken, REFRESH_TOKEN)
        REGISTER_MODEL(login_session, LoginSession, LOGIN_SESSION)
        plugin->register_trigger(std::make_shared<triggers::HistoryCommonTrigger>());

        plugin->close_for_changes();
        return plugin;
    }
}
