#include "mindnet/plugins/suggestion/SuggestionPluginFactory.h"
#include "mindnet/plugins/suggestion/validators/SuggestionValidator.h"
#include "mindnet/plugins/suggestion/validators/SuggestionReviewValidator.h"
#include "mindnet/plugins/suggestion/migrations/SuggestionSQLiteMigrationScripts.h"

namespace mindnet::plugins::suggestion
{
    const string SUGGESTION_PLUGIN_NAME = "suggestion";
    api::PluginPtr SuggestionPluginFactory::create() const
    {
        auto plugin = std::make_shared<api::Plugin>(
            SUGGESTION_PLUGIN_NAME,
            "approving changes",
            std::vector<std::string>{"suggestion"}
        );

        REGISTER_MIGRATIONS(Suggestion, SQLite)

        REGISTER_MODEL(suggestion, Suggestion, SUGGESTION)
        REGISTER_MODEL(suggestion_review, SuggestionReview, SUGGESTION_REVIEW)

        plugin->close_for_changes();
        return plugin;
    }
}
