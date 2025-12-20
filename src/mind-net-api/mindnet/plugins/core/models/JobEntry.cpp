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

#include "mindnet/plugins/core/models/JobEntry.hpp"

namespace mindnet::plugins::core::models
{
    create_model_cpp_methods(JobEntry)

    string JobEntry::validate()
    {
        using columns::JobEntryColumns;

        validator_chain_vector list{
            [this] { return testt_not_empty(name, JobEntryColumns::NAME); },
            [this] { return testt_not_empty(plugin_name, JobEntryColumns::PLUGIN_NAME); },
            [this] { return testt_not_empty(cron_expression, JobEntryColumns::CRON_EXPRESSION); },
            [this] { return test_at_least(last_run, 0, JobEntryColumns::LAST_RUN); },
            [this] { return test_at_least(next_run, 0, JobEntryColumns::NEXT_RUN); }
        };
        return util::ValidatorChain::run(list);
    }
}