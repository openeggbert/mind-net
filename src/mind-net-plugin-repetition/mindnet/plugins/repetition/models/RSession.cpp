//
// Created by robertvokac on 8/4/25.
//

#include "mindnet/plugins/repetition/models/RSession.h"

namespace mindnet::plugins::repetition::models
{
    entity_fields RSession::to_values() const
    {
        entity_fields result;
        result.push_back(id);
        result.push_back(cast64(created_at));
        result.push_back(cast64(updated_at));
        result.push_back(user_id);
        result.push_back(map_id);
        result.push_back(cloned_from_session_id);
        result.push_back(algorithm);
        result.push_back(notes);
        result.push_back(questions);
        result.push_back(scope);
        result.push_back(filter_under_note);
        result.push_back(cast64(filter_date_from));
        result.push_back(cast64(filter_date_to));
        result.push_back(filter_tag);
        result.push_back(filter_collection);
        result.push_back(selected_items);
        result.push_back(pinned);
        return result;
    }

    void RSession::from_values(const entity_fields& values)
    {
        int i = 0;

        def_helper_lambdas()

        set_id(number());
        created_at = number();
        updated_at = number();
        user_id = number();
        map_id = number();
        cloned_from_session_id = number();
        algorithm = number();
        notes = boolean();
        questions = boolean();
        scope = number();
        filter_under_note = number();
        filter_date_from = number();
        filter_date_to = number();
        filter_tag = number();
        filter_collection = number();
        selected_items = text();
        pinned = boolean();
    };

    string RSession::validate()
    {
        using columns::RSessionColumns;

        validator_chain_vector list{
            [this]
            {
                if (user_id <= 0) return std::unexpected("User ID must be positive");
                if (map_id <= 0) return std::unexpected("Map ID must be positive");
                if (algorithm <= 0) return std::unexpected("Algorithm must be positive");
                if (scope < 0) return std::unexpected("Scope must be non-negative");
                return std::expected<void>();
            },
        };
        return util::ValidatorChain::run(list);
    }
}
