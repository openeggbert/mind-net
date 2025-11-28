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

#include "crow/http_request.h"
#include "mindnet/essential/Helper.hpp"


namespace mindnet::api
{
    class IService;
    using ServicePtr = std::shared_ptr<IService>;

    struct AccessTokenContext
    {
        identification user_id{};
        std::string msg;
        int status{};
        bool system{false};

        AccessTokenContext(identification user_id, const std::string& msg, int status, bool system = false);

        AccessTokenContext(const crow::request& req, ServicePtr service_ptr);

        bool ok() const { return status == 200; }
        bool ko() const { return !ok(); }

        bool is_system() const
        {
            if (system) return true;
            return user_id == 0 && msg == "system" && status == 403;
        }

        bool is_not_system() const
        {
            return !is_system();
        }
    };
}
