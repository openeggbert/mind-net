#include "mindnet/plugins/suggestion/SuggestionPluginFactory.h"
#include "mindnet/impl/sqlite/Convertors.h"
#include "mindnet/plugins/suggestion/validators/SuggestionValidator.h"
#include "mindnet/plugins/suggestion/validators/SuggestionReviewValidator.h"

namespace mindnet::plugins::suggestion
{
    api::PluginPtr SuggestionPluginFactory::create() const
    {
        auto plugin = std::make_shared<api::Plugin>(
            "suggestion",
            "approving changes",
            std::vector<std::string>{},
            false
        );

        REGISTER_MODEL(suggestion, Suggestion, SUGGESTION)
        REGISTER_MODEL(suggestion_review, SuggestionReview, SUGGESTION_REVIEW)

        plugin->close_for_changes();
        return plugin;
    }
}
