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

#include "mindnet/plugins/slipbox/models/Task.hpp"

namespace mindnet::plugins::slipbox::models
{
    create_model_cpp_methods(Task)

    string Task::validate()
    {
        using columns::TaskColumns;

        validator_chain_vector list{
            [this] { return testt_between(title, 1, 256, TaskColumns::TITLE); },
            [this] { return testt_between(description, 0, 4096, TaskColumns::DESCRIPTION); },
            [this] { return test_between(progress, 0, 100, TaskColumns::PROGRESS); },
            [this] { return test_ne(created_by, 0, TaskColumns::CREATED_BY); },
            [this] { return test_ne(owner_id, 0, TaskColumns::OWNER_ID); },
            [this]
            {
                return test_false(due_date && start_date && due_date<start_date,
                                                                     "Due date cannot be before start date");
            }
        };
        return util::ValidatorChain::run(list);
    }
}