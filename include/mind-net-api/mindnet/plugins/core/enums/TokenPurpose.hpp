///////////////////////////////////////////////////////////////////////////////////////////////
// mind-net : A generic, high-performance REST backend framework written in modern C++.
// Copyright (C) 2025-2025 the original author or authors.
//
// This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
// without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
// See the GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License along with this program.
// If not, see <https://www.gnu.org/licenses/> or write to the Free Software Foundation, Inc.,
// 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
///////////////////////////////////////////////////////////////////////////////////////////////
#ifndef TOKENPURPOSE_H
#define TOKENPURPOSE_H

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

#endif // TOKENPURPOSE_H
