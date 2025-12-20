/*
 * MIT License
 * Copyright (c) 2025 Robert Vokac
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include "mindnet/plugins/core/models/User.hpp"

#include <regex>

#include "mindnet/essential/Configuration.hpp"

namespace mindnet::plugins::core::models
{
    create_model_cpp_methods(User)

    static const std::regex email_pattern(R"([a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,})");

    string User::validate()
    {
        using columns::UserColumns;

        validator_chain_vector list{
            [this] { return testt_not_empty(username, UserColumns::USERNAME); },
            [this] { return testt_between(username, 3, 64, UserColumns::USERNAME); },
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
                    mindnet::essential::g_configuration.registration_mode ==
                    mindnet::essential::RegistrationMode::RequiresAdminApproval
                        ? status == essential::UserStatus::Pending
                        : status == essential::UserStatus::Active,
                    mindnet::essential::g_configuration.registration_mode ==
                    mindnet::essential::RegistrationMode::RequiresAdminApproval
                        ? "status must be PENDING"
                        : "status must be ACTIVE"
                );
            }
        };

        return util::ValidatorChain::run(list);
    }
}