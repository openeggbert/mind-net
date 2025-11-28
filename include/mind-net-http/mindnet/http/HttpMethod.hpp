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

namespace mindnet::http
{
    enum class HttpMethod
    {
        GET,
        POST,
        PUT,
        DELETE,
        PATCH,
        OPTIONS,
        HEAD
    };

    inline const std::string& http_method_to_string(HttpMethod& method)
    {
        static const std::string methodNames[] = {
            "GET", "POST", "PUT", "DELETE", "PATCH", "OPTIONS", "HEAD"
        };

        static const std::string unknown = "UNKNOWN";

        size_t index = static_cast<size_t>(method);
        if (index < sizeof(methodNames) / sizeof(methodNames[0]))
        {
            return methodNames[index];
        }
        return unknown;
    }

    inline HttpMethod string_to_http_method(const std::string& method_str)
    {
        static const std::string methodNames[] = {
            "GET", "POST", "PUT", "DELETE", "PATCH", "OPTIONS", "HEAD"
        };

        for (size_t i = 0; i < sizeof(methodNames) / sizeof(methodNames[0]); i++)
        {
            if (methodNames[i] == method_str)
            {
                return static_cast<HttpMethod>(i);
            }
        }

        return HttpMethod::GET; // Default to GET if not found
    }

    inline HttpMethod crudl_to_http_method(const enums::Crudl crudl)
    {
        switch (crudl)
        {
        case enums::Crudl::CREATE:
            return HttpMethod::POST;
        case enums::Crudl::READ:
            return HttpMethod::GET;
        case enums::Crudl::UPDATE:
            return HttpMethod::PUT;
        case enums::Crudl::DELETE:
            return HttpMethod::DELETE;
        case enums::Crudl::LIST:
            return HttpMethod::GET;
        default:
            return HttpMethod::GET;
        }
    }
} // namespace mindnet::http
