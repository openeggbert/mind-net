//
// Created by robertvokac on 8/4/25.
//

#include "mindnet/models/Suggestion.h"

namespace mindnet::models
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
        operation = static_cast<enums::Crudl>(number());
        status = static_cast<enums::SuggestionStatus>(number());
        data_json = text();
        review_count = number();
    };
    string Suggestion::validate()
    {
        test_ne(from_user_id, 0, "from_user_id");
        testt_not_empty(table_name, "table_name");
        return "";
    }
}
