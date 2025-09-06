//
// Created by robertvokac on 8/4/25.
//

#include "mindnet/models/SuggestionReview.h"

namespace mindnet::models
{
    entity_fields SuggestionReview::to_values() const
    {
        entity_fields result;
        result.push_back(id);
        result.push_back(cast64(created_at));
        result.push_back(cast64(updated_at));
        result.push_back(suggestion_id);
        result.push_back(reviewer_id);
        result.push_back(cast64(decision_status));
        result.push_back(comment);
        result.push_back(reviewed_at);
        return result;
    }

    void SuggestionReview::from_values(const entity_fields& values)
    {
        int i = 0;

        def_helper_lambdas()

        set_id(number());
        created_at = number();
        updated_at = number();
        suggestion_id = number();
        reviewer_id = number();
        decision_status = static_cast<enums::DecisionStatus>(number());
        comment = text();
        reviewed_at = number();
    };

    string SuggestionReview::validate()
    {
        test_ne(reviewer_id, 0, "reviewer_id");
        return "";
    }
}
