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
        result.push_back(cast64(algorithm));
        result.push_back(notes);
        result.push_back(questions);
        result.push_back(cast64(scope));
        result.push_back(description);
        result.push_back(filter_under_note);
        result.push_back(cast64(filter_date_from));
        result.push_back(cast64(filter_date_to));
        result.push_back(filter_tag);
        result.push_back(filter_collection);
        result.push_back(selected_items);
        result.push_back(cast64(pinned));
        return result;
    }

    void RSession::from_values(const entity_fields& values)
    {
        int i = 0;

        auto number = [&values, &i]
        {
            std::cout << "values[" << i << "]" << std::endl;
            std::int64_t result = std::get<std::int64_t>(values[i++]);
            std::cout << "result[" << i << "]" << result<< std::endl;
            return result;
        };
        auto boolean = [&number] { return number() != 0; };
        auto text = [&values, &i] { return std::get<std::string>(values[i++]); };

        set_id(number());
        created_at = number();
        updated_at = number();
        user_id = number();
        map_id = number();
        cloned_from_session_id = number();
        algorithm = enums::int_to_repetition_algorithm(number());
        notes = boolean();
        questions = boolean();
        scope = enums::int_to_repetition_scope(number());
        description = text();
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
            [this] { return test_true(notes || questions, "At least one of notes or questions must be enabled."); },
            [this] { return test_true(filter_under_note == 0,  "filter_under_note is not yet supported"); },
            [this] { return test_true(filter_date_from == 0,  "filter_date_from is not yet supported"); },
            [this] { return test_true(filter_date_to == 0,  "filter_date_to is not yet supported"); },
            [this] { return test_true(filter_tag == 0,  "filter_tag is not yet supported"); },
            [this] { return test_true(filter_collection == 0,  "filter_collection is not yet supported"); },
        };
        return util::ValidatorChain::run(list);
    }
}
