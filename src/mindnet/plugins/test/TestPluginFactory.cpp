

#include "mindnet/plugins/test/TestPluginFactory.h"

#include "mindnet/impl/sqlite/Convertors.h"
#include "mindnet/plugins/test/validators/ReviewValidator.h"
#include "mindnet/plugins/test/validators/SM2StateValidator.h"

namespace mindnet::plugins::test
{
    api::PluginPtr TestPluginFactory::create() const
    {
        auto plugin = std::make_shared<api::Plugin>(
            "test",
            "SM2-Memo",
            std::vector<std::string>{"zettelkasten"},
            false
        );

        REGISTER_MODEL(review, Review, REVIEW)
        REGISTER_MODEL(sm2_state, SM2State, SM2_STATE)

        plugin->close_for_changes();
        return plugin;
    }
}
