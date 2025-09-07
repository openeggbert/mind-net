//
// Created by robertvokac on 8/4/25.
//

#include "mindnet/models/User.h"

#include <regex>

#include "mindnet/Configuration.h"

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

    string User::validate()
    {
        test_result res;
        using columns::UserColumns;

        CHECK(testt_not_empty(username, UserColumns::USERNAME))
        CHECK(testt_between(username, 5, 64, UserColumns::USERNAME))
        CHECK(testt_is_alpha_or_digit((username), UserColumns::USERNAME))
        CHECK(test_true(!isdigit(username[0]), "username must not start with a digit"))
        CHECK(test_eq(password_hash.size(), 64, UserColumns::PASSWORD_HASH))
        CHECK(test_at_most(display_name.size(), 64, UserColumns::DISPLAY_NAME))
        CHECK(testt_at_most (profile_text, 256, UserColumns::PROFILE_TEXT))

        if (!email.empty())
        {
            std::regex email_pattern(R"([a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,})");
            CHECK(test_true(std::regex_match(email, email_pattern),"Invalid email format"))
        }

        if (g_configuration.require_admin_approval_for_new_users)
        {
            CHECK(test_true(status == enums::UserStatus::PENDING, "status" " must be qual to PENDING"));
        }
        else
        {
            CHECK(test_true(status == enums::UserStatus::ACTIVE, "status" " must be qual to ACTIVE"));
        }

        return "";
    }
}
