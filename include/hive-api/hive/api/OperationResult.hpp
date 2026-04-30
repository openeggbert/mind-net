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

#define ok_result {}
#define status_405_unsupported_operation {405, "Unsupported operation."}
#define status_403_forbidden {403, "You are not authorized to access this resource."}

#include <utility>
#include <string>

namespace hive::api
{
    struct OperationResult
    {
        int status; // 0 = OK, other number = error
        std::string error; // error description, empty if OK

        OperationResult(int status_, std::string error_)
            : status(status_),
              error(std::move(error_))
        {
        }

        OperationResult() : status(0)
        {
        }

        [[nodiscard]] bool ok() const
        {
            return status == 0;
        }

        [[nodiscard]] bool ko() const
        {
            return !ok();
        }
        explicit operator bool() const noexcept {
            return ok();
        }

    };

    inline OperationResult empty_result;
}