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

namespace mindnet::plugins::dictionary::columns
{
    struct DictionaryTermFulltextColumns : model::BaseColumns
    {
        DictionaryTermFulltextColumns() = delete;

        DictionaryTermFulltextColumns(const DictionaryTermFulltextColumns&) = delete;
        DictionaryTermFulltextColumns& operator=(const DictionaryTermFulltextColumns&) = delete;

        static constexpr const char* MODEL_NAME = "dictionary_term_fulltext";

        static constexpr const char* DICTIONARY_TERM_ID = "dictionary_term_id";
        static constexpr const char* DICTIONARY_MAP_ID = "dictionary_map_id";
        static constexpr const char* DEFAULT_DICTIONARY_MAP_ID = "default_dictionary_map_id";
        static constexpr const char* TITLE_PART = "title_part";
        static constexpr const char* TITLE = "title";
        static constexpr const char* DISAMBIGUATION = "disambiguation";
        static constexpr const char* ALIAS = "alias";
    };
}