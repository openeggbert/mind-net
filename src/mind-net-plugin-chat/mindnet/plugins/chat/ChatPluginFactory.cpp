
#include "mindnet/plugins/chat/ChatPluginFactory.h"
#include "mindnet/plugins/chat/validators/CommentValidator.h"
#include "mindnet/plugins/chat/validators/DiscussionValidator.h"
#include "mindnet/plugins/chat/migrations/ChatSQLiteMigrationScripts.h"

namespace mindnet::plugins::chat
{
    const string CHAT_PLUGIN_NAME = "chat";
    api::PluginPtr ChatPluginFactory::create(std::shared_ptr<api::RepositoryFactory>& repository_factory) const
    {
        auto plugin = std::make_shared<api::Plugin>(
            CHAT_PLUGIN_NAME,
            "Slack like",
            std::vector<std::string>{"chat"}
        );
        REGISTER_MIGRATIONS(Chat, SQLite)

        REGISTER_MODEL(discussion, Discussion, DISCUSSION)
        REGISTER_MODEL(comment, Comment, COMMENT)

        plugin->close_for_changes();
        return plugin;
    }
}
