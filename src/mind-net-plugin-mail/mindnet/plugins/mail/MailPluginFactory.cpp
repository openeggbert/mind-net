

#include "mindnet/plugins/mail/MailPluginFactory.h"
#include "mindnet/plugins/mail/validators/MessageValidator.h"
#include "mindnet/plugins/mail/migrations/MailSQLiteMigrationScripts.h"

namespace mindnet::plugins::mail
{
    const string MAIL_PLUGIN_NAME = "mail";
    api::PluginPtr MailPluginFactory::create(std::shared_ptr<api::RepositoryFactory>& repository_factory) const
    {
        auto plugin = std::make_shared<api::Plugin>(
            MAIL_PLUGIN_NAME,
            "e-mail",
            std::vector<std::string>{"mail"}
        );

        REGISTER_MIGRATIONS(Mail, SQLite)

        REGISTER_MODEL(message, Message, MESSAGE)

        plugin->close_for_changes();
        return plugin;
    }
}
