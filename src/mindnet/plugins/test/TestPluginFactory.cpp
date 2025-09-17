

#include "mindnet/plugins/test/TestPluginFactory.h"

#include "mindnet/plugins/test/validators/ReviewValidator.h"
#include "mindnet/plugins/test/validators/SM2StateValidator.h"
#include "../../../../include/mindnet/plugins/test/migrations/TestSQLiteMigrationScripts.h"

namespace mindnet::plugins::test
{
    const string TEST_PLUGIN_NAME = "test";
    api::PluginPtr TestPluginFactory::create() const
    {
        auto plugin = std::make_shared<api::Plugin>(
            TEST_PLUGIN_NAME,
            "SM2-Memo",
            std::vector<std::string>{"slip_box"},
            true
        );

        REGISTER_MIGRATIONS(Test, SQLite)

        REGISTER_MODEL(review, Review, REVIEW)
        REGISTER_MODEL(sm2_state, SM2State, SM2_STATE)

        plugin->close_for_changes();
        return plugin;
    }
}
