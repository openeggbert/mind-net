

#include "mindnet/plugins/repetition/RepetitionPluginFactory.h"

#include "mindnet/plugins/repetition/validators/R2ReviewValidator.h"
#include "mindnet/plugins/repetition/validators/R2StateValidator.h"
#include "mindnet/plugins/repetition/migrations/RepetitionSQLiteMigrationScripts.h"

namespace mindnet::plugins::repetition
{
    const string REPETITION_PLUGIN_NAME = "repetition";
    api::PluginPtr RepetitionPluginFactory::create(std::shared_ptr<api::RepositoryFactory>& repository_factory) const
    {
        auto plugin = std::make_shared<api::Plugin>(
            REPETITION_PLUGIN_NAME,
            "Repetition",
            std::vector<std::string>{"repetition"},
            std::vector<std::string>{"slip_box"}
        );

        REGISTER_MIGRATIONS(Repetition, SQLite)

        REGISTER_MODEL(r2_review, R2Review, R2_REVIEW)
        REGISTER_MODEL(r2_state, R2State, R2_STATE)

        plugin->close_for_changes();
        return plugin;
    }
}
