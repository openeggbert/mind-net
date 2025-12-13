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

#include "mindnet/plugins/dictionary/models/DictionaryState4.hpp"

namespace mindnet::plugins::dictionary::models
{
    entity_fields DictionaryState4::to_values() const
    {
        return serialize_fields(*this);
    }

    void DictionaryState4::from_values(const entity_fields& values)
    {
        deserialize_fields(*this, values);
    }

    string DictionaryState4::validate()
    {
        using columns::DictionaryState4Columns;

        validator_chain_vector list{
            [this] { return test_at_least(repetitions, 0, DictionaryState4Columns::REPETITIONS); },
            [this] { return test_at_least(interval, 1, DictionaryState4Columns::INTERVAL); },
            [this] { return test_between(ef_times_100, 100, 500, DictionaryState4Columns::EF_TIMES_100); },
            [this]
            {
                return test_between(correction_factor_times_100, 0, 110, DictionaryState4Columns::CORRECTION_FACTOR_TIMES_100);
            },
            [this] { return test_between(last_quality, 0, 5, DictionaryState4Columns::LAST_QUALITY); },
            [this] { return test_at_least(last_seen_semantic_version, 1, DictionaryState4Columns::LAST_SEEN_SEMANTIC_VERSION); },
        };
        return util::ValidatorChain::run(list);
    }
}