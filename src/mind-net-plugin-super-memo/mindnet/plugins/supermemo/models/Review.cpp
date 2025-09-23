//
// Created by robertvokac on 8/4/25.
//

#include "mindnet/plugins/supermemo/models/Review.h"

namespace mindnet::plugins::supermemo::models
{
    entity_fields Review::to_values() const
    {
        entity_fields result;
        result.push_back(id);
        result.push_back(cast64(created_at));
        result.push_back(cast64(updated_at));
        result.push_back(user_id);
        result.push_back(note_id);
        result.push_back(question_id);
        result.push_back(cast64(review_date));
        result.push_back(grade);
        result.push_back(response_data);
        result.push_back(notes);
        return result;
    }

    void Review::from_values(const entity_fields& values)
    {
        int i = 0;

        def_helper_lambdas()

        set_id(number());
        created_at = number();
        updated_at = number();
        user_id = number();
        note_id = number();
        question_id = number();
        review_date = number();
        grade = number();
        response_data = text();
        notes = text();
    };

    string Review::validate()
    {
        using columns::ReviewColumns;

        validator_chain_vector list{
            [this] { return std::unexpected("Validation for Review was not yet implemented."); },
        };
        return util::ValidatorChain::run(list);
    }
}
