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
    struct JobEntryColumns : model::BaseColumns
    {
        JobEntryColumns() = delete;

        JobEntryColumns(const JobEntryColumns&) = delete;
        JobEntryColumns& operator=(const JobEntryColumns&) = delete;

        static constexpr const char* MODEL_NAME = "job_entry";

        static constexpr const char* NAME = "name";
        static constexpr const char* DESCRIPTION = "description";
        static constexpr const char* PLUGIN_NAME = "plugin_name";
        static constexpr const char* CRON_EXPRESSION = "cron_expression";
        static constexpr const char* RUN_ONCE_WHEN_MISSED = "run_once_when_missed";
        static constexpr const char* ENABLED = "enabled";
        static constexpr const char* CONFIGURATION = "configuration";
        static constexpr const char* LAST_RUN = "last_run";
        static constexpr const char* NEXT_RUN = "next_run";
    };
}
