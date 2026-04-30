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

#include "hive/model/EnumDefinition.hpp"
#include "hive/essential/EnumUtils.hpp"

namespace hive::plugins::core::enums
{
    // ---------------- ExecutionLayer enum ----------------

#define EXECUTION_LAYER_LIST(X, ENUM_NAME) \
X(Api,    0, ENUM_NAME)              \
X(App, 1, ENUM_NAME)              \
X(Db,   2, ENUM_NAME)              \
X(Http,   3, ENUM_NAME) \
X(Model,   4, ENUM_NAME) \
X(Orm,   5, ENUM_NAME) \
X(Other,   99, ENUM_NAME) \
X(Plugin,   100, ENUM_NAME)
    DECLARE_ENUM(ExecutionLayer, execution_layer, EXECUTION_LAYER_LIST)

    static constexpr const char* EXECUTION_LAYER = "EXECUTION_LAYER";
} // namespace hive::plugins::core::enums
