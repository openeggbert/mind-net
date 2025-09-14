

#include "mindnet/plugins/mail/MailPluginFactory.h"

#include "mindnet/impl/sqlite/Convertors.h"
#include "mindnet/plugins/mail/validators/MessageValidator.h"

namespace mindnet::plugins::mail
{
    api::PluginPtr MailPluginFactory::create() const
    {
        auto plugin = std::make_shared<api::Plugin>(
            "mail",
            "e-mail",
            std::vector<std::string>{},
            false
        );

        REGISTER_MODEL(message, Message, MESSAGE)

        plugin->close_for_changes();
        return plugin;
    }
}
