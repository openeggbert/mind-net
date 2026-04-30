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

#include "hive/model/EnumDefinition.hpp"

namespace hive::plugins::core::enums
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

    inline hive::model::EnumDefinition http_method_to_enum_definition()
    {
        return hive::model::EnumDefinition{
            http_method_to_string, 8, 0, 1, 2, 3, 4, 5, 6, 7
        };
    }

    static const constexpr char* HTTP_METHOD = "HTTP_METHOD";
} // namespace hive::plugins::core::enums