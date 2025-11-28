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

/**
 *
* @author <a href="mailto:robertvokac@robertvokac.com">Robert Vokac</a>
 */
#pragma once

#include "mindnet/model/BaseColumns.hpp"

namespace mindnet::plugins::core::columns
{
    struct UserColumns : model::BaseColumns
    {
        UserColumns() = delete;

        UserColumns(const UserColumns&) = delete;
        UserColumns& operator=(const UserColumns&) = delete;

        static constexpr const char* MODEL_NAME = "user";

        // Authentication fields
        static constexpr const char* USERNAME = "username";
        static constexpr const char* PASSWORD_HASH = "password_hash";
        static constexpr const char* EMAIL = "email";

        // Profile fields
        static constexpr const char* DISPLAY_NAME = "display_name";
        static constexpr const char* PROFILE_TEXT = "profile_text";

        // Access control fields
        static constexpr const char* ROLE = "role";
        static constexpr const char* STATUS = "status";

        // Tracking field
        static constexpr const char* LAST_LOGIN = "last_login";
    };
}