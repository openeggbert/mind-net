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

#pragma once


#include <string>
#include "mindnet/model/EnumDefinition.hpp"


namespace mindnet::plugins::core::enums
{
    /**
     * @enum TokenPurpose
     * @brief Enumerates different purposes a token can serve within the system.
     *
     * This enumeration defines various token purposes that determine how a token
     * can be used within the system:
     *
     * - SESSION: Used for regular user sessions
     * - API: Used for API access
     * - SERVICE: Used for service-to-service communication
     */
    enum class TokenPurpose
    {
        Session = 0,
        Api = 1,
        Service = 2
    };

    inline std::string token_purpose_to_string(const TokenPurpose purpose)
    {
        switch (purpose)
        {
        case TokenPurpose::Session:
            return "Session";
        case TokenPurpose::Api:
            return "Api";
        case TokenPurpose::Service:
            return "Service";
        default:
            return "Unknown";
        }
    }

    inline std::string token_purpose_to_string(int purpose)
    {
        return token_purpose_to_string(static_cast<TokenPurpose>(purpose));
    }

    inline TokenPurpose string_to_token_purpose(const std::string& purpose_str)
    {
        if (purpose_str == "Session") return TokenPurpose::Session;
        if (purpose_str == "Api") return TokenPurpose::Api;
        if (purpose_str == "Service") return TokenPurpose::Service;
        throw std::runtime_error("Invalid token purpose: " + purpose_str);
    }

    inline mindnet::model::EnumDefinition token_purpose_to_enum_definition()
    {
        return mindnet::model::EnumDefinition{
            token_purpose_to_string, 3, 0, 1, 2
        };
    }
} // namespace mindnet::plugins::core::enums

