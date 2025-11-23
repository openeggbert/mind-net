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


namespace mindnet::http
{
    struct UserCredentials
    {
        std::string username;
        std::string password;
        std::string error;

        UserCredentials(const crow::request& req)
        {
            auto auth = req.get_header_value("Authorization"); // Basic base64(username:password)
            if (auth.empty())
            {
                error = "Missing Authorization header.";
                return;
            }
            if (auth.rfind("Basic ", 0) != 0)
            {
                error = "Invalid Authorization header.";
                return;
            }

            std::string creds;
            try
            {
                creds = crow::utility::base64decode(auth.substr(6));
            }
            catch (const std::exception& e)
            {
                error = "Invalid base64 encoding in Authorization header.";
                return;
            }

            auto sep = creds.find(':');
            if (sep == std::string::npos)
            {
                error = "Invalid credentials format.";
                return;
            }

            username = creds.substr(0, sep);
            password = creds.substr(sep + 1);
        }
    };
}

