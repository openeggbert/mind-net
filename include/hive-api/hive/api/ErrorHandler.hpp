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

#include <functional>
#include <memory>

#include "AccessTokenContext.hpp"
#include "OperationResult.hpp"
#include "hive/essential/DatabaseType.hpp"
#include "hive/essential/Helper.hpp"
#include "hive/util/triple.hpp"
#include "hive/plugins/core/models/Error.hpp"

// 🔐 Validator as a Security Gate
// Validator does more than just check data — it acts as protection against unauthorized access:
//
// Authentication: Is the user logged in?
// Authorization: Does the user have the right to read/modify the given entity?
// Integrity: Does the operation preserve the system’s logic?
// Consistency: Are key dependencies present (e.g., a parent note)?

namespace hive::api
{
    using ErrorHandlerResult = util::triple<identification, std::string, int>;
    class ErrorHandler
    {
    public:
        virtual ~ErrorHandler() = default;
        virtual ErrorHandlerResult report_error(plugins::core::models::Error& error) = 0;
    };
}