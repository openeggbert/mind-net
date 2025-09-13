//
// Created by robertvokac on 8/4/25.
//

#include "mindnet/plugins/zettelkasten/models/Question.h"

namespace mindnet::plugins::zettelkasten::models
{
    entity_fields Question::to_values() const
    {
        entity_fields result;
        result.push_back(id);
        result.push_back(cast64(created_at));
        result.push_back(cast64(updated_at));
        result.push_back(note_id);
        result.push_back(question_text);
        result.push_back(answers_json);
        return result;
    }

    void Question::from_values(const entity_fields& values)
    {
        int i = 0;

        def_helper_lambdas()

        set_id(number());
        created_at = number();
        updated_at = number();
        note_id = number();
        question_text = text();
        answers_json = text();
    };

    string Question::validate()
    {
        using columns::QuestionColumns;

        validator_chain_vector list{
            [this] { return test_ne(note_id, 0, QuestionColumns::NOTE_ID); },
            [this] { return testt_between(question_text, 5, 256, QuestionColumns::QUESTION_TEXT); }
        };
        return ValidatorChain::run(list);
    }
}
