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
    struct ErrorColumns : model::BaseColumns
    {
        ErrorColumns() = delete;

        ErrorColumns(const ErrorColumns&) = delete;
        ErrorColumns& operator=(const ErrorColumns&) = delete;

        static constexpr const char* MODEL_NAME = "error";

        /* ------------------------------------------------------------------
         * Identity
         * ------------------------------------------------------------------
         * External and internal identification of the error record
         */
        static constexpr const char* HUMAN_IDENTIFICATION = "human_identification"; // Human-readable error ID (shown to user)
        static constexpr const char* UUID = "uuid"; // Internal unique identifier (e.g. UUIDv7)

        /* ------------------------------------------------------------------
         * Classification / result
         * ------------------------------------------------------------------
         * Severity and HTTP impact of the error
         */
        static constexpr const char* SEVERITY = "severity"; // info | warn | error | fatal
        static constexpr const char* HTTP_STATUS = "http_status"; // HTTP status code (200 / 400 / 500 / ...)

        /* ------------------------------------------------------------------
         * Origin
         * ------------------------------------------------------------------
         * Where the error originated from
         */
        static constexpr const char* ORIGIN = "origin"; // api | system | job | trigger

        // build-time layer (CMake target / Mind Net module)
        static constexpr const char* LAYER = "layer";

        /* ------------------------------------------------------------------
         * System location
         * ------------------------------------------------------------------
         * Where in the Mind Net architecture the error occurred
         */
        static constexpr const char* PLUGIN = "plugin"; // dictionary, core, auth, ...
        static constexpr const char* OPERATION = "operation"; // LIST / CREATE / UPDATE / DELETE

        /* ------------------------------------------------------------------
         * Code location (C++)
         * ------------------------------------------------------------------
         * Precise location in the C++ source code
         */
        static constexpr const char* CPP_NAMESPACE = "cpp_namespace";
        static constexpr const char* CPP_CLASS = "cpp_class";
        static constexpr const char* CPP_SYMBOL = "cpp_symbol"; // function / method / lambda

        /* ------------------------------------------------------------------
         * Technical context
         * ------------------------------------------------------------------
         * Low-level technical context related to the failure
         */
        static constexpr const char* SQL_QUERY = "sql_query"; // SQL statement that failed (if applicable)

        /* ------------------------------------------------------------------
         * Request / user context
         * ------------------------------------------------------------------
         * Context of the request and the user
         */
        static constexpr const char* USER_ID = "user_id";
        static constexpr const char* REQUEST_IDENTIFICATION = "request_identification"; // Request correlation / tracing ID

        /* ------------------------------------------------------------------
         * Payload
         * ------------------------------------------------------------------
         * Human-readable error information
         */
        static constexpr const char* MESSAGE = "message";
        static constexpr const char* EXCEPTION_TYPE = "exception_type"; // std::runtime_error, SQLite::Exception, ...
        static constexpr const char* EXCEPTION_MESSAGE = "exception_message";  // exception.what()

        /* ------------------------------------------------------------------
         * Lifecycle
         * ------------------------------------------------------------------
         * Error handling state from the developer perspective
         */
        static constexpr const char* HANDLED = "handled"; // 0 = new, 1 = acknowledged, 2 = fixed

        /* ------------------------------------------------------------------
         * Versioning
         * ------------------------------------------------------------------
         * Mind Net version in which the error occurred
         */
        static constexpr const char* MINDNET_VERSION = "mindnet_version";
    };
}
