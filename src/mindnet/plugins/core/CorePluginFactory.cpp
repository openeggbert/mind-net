#include "mindnet/plugins/core/CorePluginFactory.h"

#include "mindnet/plugins/core/validators/HistoryValidator.h"
#include "mindnet/plugins/core/validators/TeamMemberValidator.h"
#include "mindnet/plugins/core/validators/TeamValidator.h"
#include "mindnet/plugins/core/validators/UserValidator.h"

namespace mindnet::plugins::core
{
    api::PluginPtr CorePluginFactory::create() const
    {
        auto plugin = std::make_shared<api::Plugin>(
            "core",
            "core models",
            std::vector<std::string>{},
            false
        );

        REGISTER_MODEL(user, User, USER)
        REGISTER_MODEL(team, Team, TEAM)
        REGISTER_MODEL(team_member, TeamMember, TEAM_MEMBER)
        REGISTER_MODEL(history, History, HISTORY)

        plugin->close_for_changes();
        return plugin;
    }
}
