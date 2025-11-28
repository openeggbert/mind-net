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
    entity_fields JobEntry::to_values() const
    {
        entity_fields result;
        result.emplace_back(id);
        result.emplace_back(cast64(created_at));
        result.emplace_back(cast64(updated_at));
        result.emplace_back(name);
        result.emplace_back(description);
        result.emplace_back(plugin_name);
        result.emplace_back(cron_expression);
        result.emplace_back(run_once_when_missed);
        result.emplace_back(enabled);
        result.emplace_back(configuration);
        result.emplace_back(cast64(last_run));
        result.emplace_back(cast64(next_run));
        return result;
    }

    void JobEntry::from_values(const entity_fields& values)
    {
        int i = 0;

        def_helper_lambdas()

        set_id(number());
        created_at = number();
        updated_at = number();
        name = text();
        description = text();
        plugin_name = text();
        cron_expression = text();
        run_once_when_missed = boolean();
        enabled = boolean();
        configuration = text();
        last_run = number();
        next_run = number();
    }

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
