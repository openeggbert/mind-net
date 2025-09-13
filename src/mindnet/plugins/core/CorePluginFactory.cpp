

#include "mindnet/plugins/core/CorePluginFactory.h"

#include "mindnet/impl/sqlite/Convertors.h"
#include "mindnet/plugins/core/models/User.h"
#include "mindnet/plugins/core/validators/UserValidator.h"

namespace mindnet::plugins::core {
api::PluginPtr CorePluginFactory::create() const
{
    auto plugin = std::make_shared<api::Plugin>(
        "core",
        "core models",
        std::vector<std::string>{},
        false
    );
    plugin->register_model(
        models::USER_DEFINITION,
        std::make_shared<validators::UserValidator>(),
        impl::sqlite::request_to_entity_fields_user);

    plugin->close_for_changes();
    return plugin;
}
}
