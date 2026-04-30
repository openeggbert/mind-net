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

#include "hive/plugins/slipbox/triggers/BeforeUpdateContentTrigger.hpp"

#include "hive/essential/Global.hpp"
#include "hive/api/AccessTokenContext.hpp"
#include "hive/plugins/slipbox/models/Content.hpp"
#include <iostream>
#include <string>
#include <vector>
#include <regex>
#include <unordered_set>

#include "hive/plugins/slipbox/models/Note.hpp"
#include "hive/util/Utils.hpp"

namespace hive::plugins::slipbox::triggers
{
    using_loggers()

    BeforeUpdateContentTrigger::BeforeUpdateContentTrigger()
        : Trigger(
            "BeforeUpdateContentTrigger",
            "Modifies semantic_version and change_ratio_times_100 if needed.",
            1000,
            {essential::Crudl::Update},
            api::TriggerPhase::Before,
            "content"
        )
    {
    }

    using std::vector;

    vector<string> tokenize(const string& s)
    {
        vector<string> out;
        string cur;
        for (char c : s)
        {
            if (isalnum((unsigned char)c)) cur.push_back(tolower((unsigned char)c));
            else
            {
                if (!cur.empty())
                {
                    out.push_back(cur);
                    cur.clear();
                }
            }
        }
        if (!cur.empty()) out.push_back(cur);
        return out;
    }

    size_t edit_distance(const vector<string>& A, const vector<string>& B)
    {
        const size_t n = A.size(), m = B.size();
        if (n < m) return edit_distance(B, A); // ensures m <= n in memory
        vector<size_t> prev(m + 1), cur(m + 1);
        iota(prev.begin(), prev.end(), 0); // 0..m
        for (size_t i = 1; i <= n; ++i)
        {
            cur[0] = i;
            for (size_t j = 1; j <= m; ++j)
            {
                size_t cost = (A[i - 1] == B[j - 1]) ? 0 : 1;
                cur[j] = std::min({
                        prev[j] + 1, // delete
                        cur[j - 1] + 1, // insert
                        prev[j - 1] + cost
                    } // substitute
                );
            }
            swap(prev, cur);
        }
        return prev[m];
    }

    struct DiffPercent
    {
        double wer_percent; // change vs. original (%)
        double similarity_percent; // 1 - d/max(n,m) in %
        size_t edit_distance;
        size_t old_word_count;
        size_t new_word_count;
    };

    DiffPercent compareTexts(const string& oldText, const string& newText)
    {
        auto A = tokenize(oldText);
        auto B = tokenize(newText);
        size_t n = A.size(), m = B.size();
        size_t d = edit_distance(A, B);
        double wer = (n == 0) ? (m ? 100.0 : 0.0) : 100.0 * (double)d / (double)n;
        double sim = (std::max(n, m) == 0) ? 100.0 : 100.0 * (1.0 - (double)d / (double)std::max(n, m));
        return {wer, sim, d, A.size(), B.size()};
    }

    void BeforeUpdateContentTrigger::run_before_or_after(
        hive::essential::Crudl operation,
        int stack_depth,
        api::OperationResult& validation_result,
        api::OperationResult& action_result,
        const hive::model::ModelDefinition def,
        identification user_id,
        identification id,
        entity_fields& fields,
        entity_fields& old_fields,
        const orm::QueryParams query_params)
    {
        if (action_result.ko())
        {
            return;
        }
        models::Content new_content;
        models::Content old_content;
        new_content.from_values(fields);
        old_content.from_values(old_fields);

        if (new_content.value == old_content.value)
        {
            info << "Content value unchanged. Exiting trigger." << commit;
            return;
        }

        auto diff = compareTexts(old_content.value, new_content.value);

        if (diff.old_word_count <= 1 && diff.new_word_count <= 1)
        {
            new_content.change_ratio = 100;
            new_content.semantic_version++;
            fields = new_content.to_values();
            info << "Content changed by " << new_content.change_ratio << "%, semantic_version -> "
                << new_content.semantic_version << commit;
            return;
        }

        if (std::abs((int)diff.old_word_count - (int)diff.new_word_count) > 50)
        {
            // huge change, no need to calculate ED
            new_content.change_ratio = 100;
            new_content.semantic_version++;
            info << "Content changed by " << new_content.change_ratio << "%, semantic_version -> "
                << new_content.semantic_version << commit;
            fields = new_content.to_values();
            return;
        }

        auto& wer = diff.wer_percent;

        // WER is in percent. 20% = 20.0
        bool big_change =
            (wer >= 20.0) ||
            (diff.old_word_count < 6 && diff.new_word_count < 12) ||
            (std::abs((int)diff.old_word_count - (int)diff.new_word_count) > 50);

        if (big_change)
        {
            new_content.change_ratio = std::min(100, (int)std::round(wer));
            new_content.semantic_version++;
            fields = new_content.to_values();
            info << "Content changed by " << wer << "%, semantic_version -> "
                << new_content.semantic_version << commit;
        }
    }
}