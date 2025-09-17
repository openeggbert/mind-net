#include "mindnet/plugins/core/CorePluginFactory.h"

#include "../../../../include/mindnet/plugins/core/migrations/CoreSQLiteMigrationScripts.h"
#include "mindnet/plugins/core/triggers/HistoryCommonTrigger.h"
#include "mindnet/plugins/core/validators/HistoryValidator.h"
#include "mindnet/plugins/core/validators/TeamMemberValidator.h"
#include "mindnet/plugins/core/validators/TeamValidator.h"
#include "mindnet/plugins/core/validators/UserValidator.h"

namespace mindnet::plugins::core
{
    const string CORE_PLUGIN_NAME = "core";
    api::PluginPtr CorePluginFactory::create() const
    {
        auto plugin = std::make_shared<api::Plugin>(
            CORE_PLUGIN_NAME,
            "core models",
            std::vector<std::string>{},
            false
        );
        REGISTER_MIGRATIONS(Core, SQLite)

        REGISTER_MODEL(user, User, USER)
        REGISTER_MODEL(team, Team, TEAM)
        REGISTER_MODEL(team_member, TeamMember, TEAM_MEMBER)
        REGISTER_MODEL(history, History, HISTORY)
        plugin->register_trigger(std::make_shared<triggers::HistoryCommonTrigger>());

        plugin->close_for_changes();
        return plugin;
    }
}
