//
// Created by robertvokac on 8/4/25.
//

#include "mindnet/models/Discussion.h"

namespace mindnet::models
{
    entity_fields Discussion::to_values() const
    {
        entity_fields result;
        result.push_back(id);
        result.push_back(cast64(created_at));
        result.push_back(cast64(updated_at));
        result.push_back(team_id);
        result.push_back(title);
        result.push_back(created_by);
        result.push_back(is_pinned);
        result.push_back(is_archived);
        return result;
    }

    void Discussion::from_values(const entity_fields& values)
    {
        int i = 0;

        def_helper_lambdas()

        set_id(number());
        created_at = number();
        updated_at = number();
        team_id = number();
        title = text();
        created_by = number();
        is_pinned = number();
        is_archived = number();
    };

    string Discussion::validate()
    {
        test_ne(team_id, 0, "team_id");
        test_between(title.length(), 1, 80, "title");
        for (char ch:title)
        {
            if (!isdigit(ch) && !isalpha(ch) && ch != '-' && ch != '_')
            {
                return "title must contain only letters, digits, - and _";
            }
            if (isalpha(ch) && !islower(ch))
            {
                return "title must contain only lowercase letters";
            }
        }
        test_ne(created_by, 0, "created_by");

        return "";
    }
}
