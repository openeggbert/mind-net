//
// Created by robertvokac on 8/4/25.
//

#include "mindnet/plugins/slipbox/models/Question.hpp"

namespace mindnet::plugins::slipbox::models
{
    QuestionAnswer::QuestionAnswer(const std::string& answer_to_be_parsed)
    {
        std::stringstream ss(answer_to_be_parsed);
        std::string item;
        bool is_first = true;

        while (std::getline(ss, item, *ANSWER_SEPARATOR))
        {
            if (is_first)
            {
                value.emplace_back(item, true);
                is_first = false;
            }
            else
            {
                bool is_correct = !item.empty() && item[0] == '+';
                if (is_correct)
                {
                    item = item.substr(1);
                }
                value.emplace_back(item, is_correct);
            }
        }
    }

    entity_fields Question::to_values() const
    {
        entity_fields result;
        result.push_back(id);
        result.push_back(cast64(created_at));
        result.push_back(cast64(updated_at));
        result.push_back(note_id);
        result.push_back(question_text);
        result.push_back(answers);
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
        answers = text();
    };

    string Question::validate()
    {
        using columns::QuestionColumns;

        validator_chain_vector list{
            [this] { return test_ne(note_id, 0, QuestionColumns::NOTE_ID); },
            [this] { return testt_between(question_text, 5, 256, QuestionColumns::QUESTION_TEXT); }
        };
        return util::ValidatorChain::run(list);
    }
}
