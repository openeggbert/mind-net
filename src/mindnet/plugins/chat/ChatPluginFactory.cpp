

#include "mindnet/plugins/chat/ChatPluginFactory.h"
#include "mindnet/impl/sqlite/Convertors.h"
#include "mindnet/plugins/chat/validators/CommentValidator.h"
#include "mindnet/plugins/chat/validators/DiscussionValidator.h"

namespace mindnet::plugins::chat
{
    api::PluginPtr ChatPluginFactory::create() const
    {
        auto plugin = std::make_shared<api::Plugin>(
            "chat",
            "Slack like",
            std::vector<std::string>{},
            false
        );
        REGISTER_MODEL(discussion, Discussion, DISCUSSION)
        REGISTER_MODEL(comment, Comment, COMMENT)

        plugin->close_for_changes();
        return plugin;
    }
}
