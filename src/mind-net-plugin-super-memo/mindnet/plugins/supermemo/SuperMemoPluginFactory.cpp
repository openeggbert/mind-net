

#include "mindnet/plugins/supermemo/SuperMemoPluginFactory.h"

#include "mindnet/plugins/supermemo/validators/ReviewValidator.h"
#include "mindnet/plugins/supermemo/validators/SM2StateValidator.h"
#include "mindnet/plugins/supermemo/migrations/SuperMemoSQLiteMigrationScripts.h"

namespace mindnet::plugins::supermemo
{
    const string TEST_PLUGIN_NAME = "super_memo";
    api::PluginPtr SuperMemoPluginFactory::create(std::shared_ptr<api::RepositoryFactory>& repository_factory) const
    {
        auto plugin = std::make_shared<api::Plugin>(
            TEST_PLUGIN_NAME,
            "SM18-Memo",
            std::vector<std::string>{"super_memo"},
            std::vector<std::string>{"slip_box"}
        );

        REGISTER_MIGRATIONS(SuperMemo, SQLite)

        REGISTER_MODEL(review, Review, REVIEW)
        REGISTER_MODEL(sm2_state, SM2State, SM2_STATE)

        plugin->close_for_changes();
        return plugin;
    }
}
