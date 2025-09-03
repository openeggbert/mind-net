//
// Created by robertvokac on 8/4/25.
//

#include "mindnet/models/User.h"

namespace mindnet::models
{
    entity_fields User::to_values() const
    {
        entity_fields result;
        result.push_back(id);
        result.push_back(cast64(created_at));
        result.push_back(cast64(updated_at));
        //
        result.push_back(username);
        result.push_back(password_hash);
        result.push_back(display_name);
        result.push_back(cast64(role));
        result.push_back(profile_text);
        result.push_back(cast64(last_login));
        result.push_back(email);
        result.push_back(cast64(status));
        return result;
    }

    void User::from_values(const entity_fields& values)
    {
        int i = 0;

        def_helper_lambdas()

        set_id(number());
        created_at = number();
        updated_at = number();
        //
        username = text();
        password_hash = text();
        display_name = text();
        role = static_cast<enums::UserRole>(number());
        profile_text = text();
        last_login = number();
        email = text();
        status = static_cast<enums::UserStatus>(number());
    };
}
