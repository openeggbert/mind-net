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

#include "mindnet/plugins/core/models/JobRun.hpp"

namespace mindnet::plugins::core::models
{
    entity_fields JobRun::to_values() const
    {
        entity_fields result;
        result.emplace_back(id);
        result.emplace_back(cast64(created_at));
        result.emplace_back(cast64(updated_at));
        result.emplace_back(job_entry_id);
        result.emplace_back(cast64(started_at));
        result.emplace_back(cast64(finished_at));
        result.emplace_back(success);
        result.emplace_back(message);
        return result;
    }

    void JobRun::from_values(const entity_fields& values)
    {
        int i = 0;

        def_helper_lambdas()

        set_id(number());
        created_at = number();
        updated_at = number();
        job_entry_id = number();
        started_at = number();
        finished_at = number();
        success = boolean();
        message = text();
    }

    string JobRun::validate()
    {
        using columns::JobRunColumns;

        validator_chain_vector list{
            [this] { return test_at_least(job_entry_id, 1, JobRunColumns::JOB_ENTRY_ID); },
            [this] { return test_at_least(started_at, 0, JobRunColumns::STARTED_AT); },
            [this] { return test_at_least(finished_at, 0, JobRunColumns::FINISHED_AT); },
            [this] { return test_true(finished_at >= started_at || finished_at == 0, "Finished at must be at least the same as Started at");}
        };
        return util::ValidatorChain::run(list);
    }
}
