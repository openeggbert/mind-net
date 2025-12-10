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

#include "mindnet/plugins/dictionary/models/DictionaryTagType.hpp"

namespace mindnet::plugins::dictionary::models
{
    entity_fields DictionaryTagType::to_values() const
    {
        return serialize_fields(*this);
    }

    void DictionaryTagType::from_values(const entity_fields& values)
    {
        deserialize_fields(*this, values);
    };

    std::string is_tag_type_title_valid(const std::string& title)
    {
        if (title.empty())
            return "title must not be empty";

        if (title[0] == '-')
            return "title must not start with '-'";

        char prev = '\0';

        for (char c : title)
        {
            // forbid two consecutive dashes
            if (c == '-' && prev == '-')
                return "tag type cannot contain two consecutive dash characters";

            // allow: lowercase letters
            if (c >= 'a' && c <= 'z') { prev = c; continue; }

            // allow: digits
            if (c >= '0' && c <= '9') { prev = c; continue; }

            // allow: dash
            if (c == '-') { prev = c; continue; }

            // otherwise not allowed
            return std::string("invalid character in tag type: '") + c + "'";
        }

        return "";
    };

    string DictionaryTagType::validate()
    {
        using columns::DictionaryTagTypeColumns;

        string is_tag_type_title_valid_result = is_tag_type_title_valid(title);
        validator_chain_vector list{
            [this] { return test_ne(dictionary_map_id, 0, DictionaryTagTypeColumns::DICTIONARY_MAP_ID); },
            [this] { return testt_between(title, 1, 64, DictionaryTagTypeColumns::TITLE); },
            [&is_tag_type_title_valid_result] { return test_true(is_tag_type_title_valid_result.empty(), "Tag type is not valid: " + is_tag_type_title_valid_result); },
        };
        return util::ValidatorChain::run(list);
    }
}