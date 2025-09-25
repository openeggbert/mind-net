///////////////////////////////////////////////////////////////////////////////////////////////
// mind-net : A generic, high-performance REST backend framework written in modern C++.
// Copyright (C) 2025-2025 the original author or authors.
//
// This program is free software: you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation, either version 3
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program. If not, see 
// <https://www.gnu.org/licenses/> or write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
///////////////////////////////////////////////////////////////////////////////////////////////
#ifndef HTTP_METHOD_H
#define HTTP_METHOD_H

#include <string>

#include "mindnet/model/EnumDefinition.h"

namespace mindnet::plugins::core::enums
{
    /**
     *
     * @author robertvokac
     */
    enum class HttpMethod
    {
        Undefined = 0, GET = 1, POST = 2, PUT = 3, DELETE = 4, PATCH = 5, HEAD = 6, OPTIONS = 7
    };

    inline std::string http_method_to_string(HttpMethod method)
    {
        switch (method)
        {
        case HttpMethod::Undefined:
            return "Undefined";
        case HttpMethod::GET:
            return "GET";
        case HttpMethod::POST:
            return "POST";
        case HttpMethod::PUT:
            return "PUT";
        case HttpMethod::DELETE:
            return "DELETE";
        case HttpMethod::PATCH:
            return "PATCH";
        case HttpMethod::HEAD:
            return "HEAD";
        case HttpMethod::OPTIONS:
            return "OPTIONS";
        default:
            return "Unknown";
        }
    }

    inline std::string http_method_to_string(int method)
    {
        return http_method_to_string(static_cast<HttpMethod>(method));
    }

    inline HttpMethod string_to_http_method(const std::string& method)
    {
        if (method == "GET") return HttpMethod::GET;
        if (method == "POST") return HttpMethod::POST;
        if (method == "PUT") return HttpMethod::PUT;
        if (method == "DELETE") return HttpMethod::DELETE;
        if (method == "PATCH") return HttpMethod::PATCH;
        if (method == "HEAD") return HttpMethod::HEAD;
        if (method == "OPTIONS") return HttpMethod::OPTIONS;
        return HttpMethod::Undefined;
    }

    inline mindnet::model::EnumDefinition http_method_to_enum_definition()
    {
        return mindnet::model::EnumDefinition{
            http_method_to_string, 8, 0, 1, 2, 3, 4, 5, 6, 7
        };
    }

    static const constexpr char* HTTP_METHOD = "HTTP_METHOD";
} // namespace mindnet::plugins::core::enums

#endif // HTTP_METHOD_H
