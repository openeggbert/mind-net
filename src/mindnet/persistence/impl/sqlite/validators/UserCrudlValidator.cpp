//
// Created by robertvokac on 8/6/25.
//


#include "mindnet/persistence/impl/sqlite/validators/UserCrudlValidator.h"

#include <regex>
#include "crow/common.h"
#include "mindnet/Configuration.h"
#include "mindnet/Global.h"
#include "mindnet/models/User.h"
#include "mindnet/persistence/Persistence.h"
#define test_true(condition, error) if ( ! ( condition ) ) return error;
//
#define test_eq(number, eq_to, field) test_true(number == eq_to, field " must be qual to " + std::to_string(eq_to))
#define test_at_least(number, min_value, field) test_true(number >= min_value, field " must not be less than " + std::to_string(min_value))
#define test_at_most(number, max_value, field) test_true(number <= max_value, field " must not be greater than " + std::to_string(max_value))
#define test_between(number, min_value, max_value, field) test_at_least(number, min_value, field) test_at_most(number, max_value, field)
//
#define testt_at_least(text, min_value, field) test_at_least(text .size(), min_value, field)
#define testt_at_most(text, max_value, field) test_at_most(text .size(), max_value, field)
#define testt_between(text, min_value, max_value, field) test_between(text .size(), min_value, max_value, field)
//
#define testt_not_empty(text, field) test_true(!text.empty(), field " must not be empty")


namespace mindnet::persistence::impl::sqlite::validators
{
    using impl::sqlite::validators::UserCrudlValidator;

    string validate(models::User& user, enums::Crudl& crudl)
    {
        bool create = crudl == enums::Crudl::CREATE;
        bool update = crudl == enums::Crudl::UPDATE;

        if (!create && !update) return "crudl must be CREATE or UPDATE";

        testt_not_empty(user.username, "username");
        testt_between(user.username, 5, 64, "username");
        for (char ch:user.username) if (!isdigit(ch) && !isalpha(ch)) return "username must contain only letters and digits";
        if (isdigit(user.username[0])) return "username must not start with a digit";

        if (create) test_eq(user.password_hash.size(), 64, "password_hash");
        test_at_most(user.display_name.size(), 64, "display_name");
        //
        if (create) {
        enums::UserRole default_role = g_configuration.default_user_role;
        if ( ! ( user.role == default_role) ) return "role" " must be qual to " + enums::user_role_to_string(default_role);
        }

        if ( ! ( user.profile_text .size() <= 256 ) ) return "profile_text" " must not be greater than " + std::to_string(256);

        std::regex email_pattern(R"([a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,})");
        if (!std::regex_match(user.email, email_pattern))
        {
            return "Invalid email format";
        }
        if (create)
        {
            if (g_configuration.require_admin_approval_for_new_users)
            {
                if ( ! ( user.status == enums::UserStatus::PENDING) ) return "status" " must be qual to PENDING";
            } else
            {
                if ( ! ( user.status == enums::UserStatus::ACTIVE) ) return "status" " must be qual to ACTIVE";
            }
        }



        return "";
    }
    validator_result UserCrudlValidator::can_create(db_ d, entity_fields& ef) const
    {
        models::User user;
        user.from_values(ef);
        err << user << commit;

        string str = validate(user);
        if (!str.empty()) return validator_result(400, str);

        string error;
        http::QueryParams query_params;
        query_params.filters.emplace("name", user.username);
        if (!d->list(query_params, models::USER_DEFINITION, error).empty())
        {
            return validator_result(409, "Name name already exists");
        }




        if (map.team_rights < 0 || map.team_rights > 7)
        {
            return "team_rights must be between 0 and 7";
        }
        if (map.other_rights < 0 || map.other_rights > 7)
        {
            return "other_rights must be between 0 and 7";
        }
        if (map.owner_rights != castint(enums::AccessRight::READ_WRITE_DELETE))
        {
            //todo
            return "Owner rights must be Read+Write+Delete. This is temporary.";
        }
        if (map.team_rights != castint(enums::AccessRight::NONE))
        {
            return "Team rights must be NONE. This is temporary.";
        }
        if (map.other_rights != castint(enums::AccessRight::NONE))
        {
            return "Other rights must be NONE. This is temporary.";
        }
        return "";
    }

    validator_result UserCrudlValidator::can_read(db_ d, int id) const
    {
        return "";
    }

    validator_result UserCrudlValidator::can_update(db_ d, entity_fields& ef) const
    {
        return "";
    }

    validator_result UserCrudlValidator::can_delete(db_ d, int id) const
    {
        return "";
    }

    validator_result UserCrudlValidator::can_list(db_ d, std::map<std::string, std::string>& filter) const
    {
        return "";
    }

    string UserCrudlValidator::get_model_name() const
    {
        return "user";
    }
}
