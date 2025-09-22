//
// Created by robertvokac on 8/4/25.
//

#include "mindnet/other/plugins/suggestion/models/Suggestion.h"

namespace mindnet::plugins::suggestion::models
{
    entity_fields Suggestion::to_values() const
    {
        entity_fields result;
        result.push_back(id);
        result.push_back(cast64(created_at));
        result.push_back(cast64(updated_at));
        result.push_back(parent_suggestion_id);
        result.push_back(from_user_id);
        result.push_back(table_name);
        result.push_back(cast64(operation));
        result.push_back(cast64(status));
        result.push_back(data_json);
        result.push_back(review_count);
        return result;
    }

    void Suggestion::from_values(const entity_fields& values)
    {
        int i = 0;

        def_helper_lambdas()

        set_id(number());
        created_at = number();
        updated_at = number();
        parent_suggestion_id = number();
        from_user_id = number();
        table_name = text();
        operation = static_cast<core::enums::Crudl>(number());
        status = static_cast<enums::SuggestionStatus>(number());
        data_json = text();
        review_count = number();
    };

    string Suggestion::validate()
    {
        using columns::SuggestionColumns;

        validator_chain_vector list{
            [this] { return test_ne(from_user_id, 0, SuggestionColumns::FROM_USER_ID); },
            [this] { return testt_not_empty(table_name, SuggestionColumns::TABLE_NAME); },
        };
        return util::ValidatorChain::run(list);
    }
}
