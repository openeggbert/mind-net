//
// Created by robertvokac on 8/4/25.
//

#include "mindnet/plugins/core/models/User.h"

#include <regex>

#include "mindnet/essential/Configuration.h"

namespace mindnet::plugins::core::models
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
        role = static_cast<mindnet::core::UserRole>(number());
        profile_text = text();
        last_login = number();
        email = text();
        status = static_cast<enums::UserStatus>(number());
    };

    static const std::regex email_pattern(R"([a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,})");

    string User::validate()
    {
        using columns::UserColumns;

        validator_chain_vector list{
            [this] { return testt_not_empty(username, UserColumns::USERNAME); },
            [this] { return testt_between(username, 5, 64, UserColumns::USERNAME); },
            [this] { return testt_is_alpha_or_digit(username, UserColumns::USERNAME); },
            [this] { return test_true(!isdigit(username[0]), "username must not start with a digit"); },
            [this] { return test_eq(password_hash.size(), 64, UserColumns::PASSWORD_HASH); },
            [this] { return test_at_most(display_name.size(), 64, UserColumns::DISPLAY_NAME); },
            [this] { return testt_at_most(profile_text, 256, UserColumns::PROFILE_TEXT); },
            [this]
            {
                if (email.empty()) return util::test_result{};
                return test_true(
                    std::regex_match(email, email_pattern),
                    "Invalid email format");
            },
            [this]
            {
                return test_true(
                    mindnet::core::g_configuration.registration_mode == mindnet::core::RegistrationMode::RequiresAdminApproval
                        ? status == enums::UserStatus::Pending
                        : status == enums::UserStatus::Active,
                    mindnet::core::g_configuration.registration_mode == mindnet::core::RegistrationMode::RequiresAdminApproval
                        ? "status must be PENDING"
                        : "status must be ACTIVE"
                );
            }
        };

        return util::ValidatorChain::run(list);
    }
}
