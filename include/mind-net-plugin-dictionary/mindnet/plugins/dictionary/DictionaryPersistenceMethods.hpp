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

#pragma once

#include <utility>

#include "mindnet/api/PersistenceMethods.hpp"
#include "mindnet/api/IPersistence.hpp"
#include "mindnet/api/OperationResult.hpp"
#include "mindnet/api/RequestContext.hpp"
#include "mindnet/plugins/core/enums/SingleRight.hpp"
#include "mindnet/api/AccessTokenContext.hpp"
#include "mindnet/api/PersistenceMethods.hpp"
#include "mindnet/plugins/dictionary/models/DictionaryFlag.hpp"
#include "mindnet/plugins/dictionary/models/DictionaryLink.hpp"
#include "mindnet/plugins/dictionary/models/DictionaryMap.hpp"
#include "mindnet/plugins/dictionary/models/DictionaryNote.hpp"
#include "mindnet/plugins/dictionary/models/DictionaryReview.hpp"
#include "mindnet/plugins/dictionary/models/DictionarySource.hpp"
#include "mindnet/plugins/dictionary/models/DictionarySourceType.hpp"
#include "mindnet/plugins/dictionary/models/DictionaryState18.hpp"
#include "mindnet/plugins/dictionary/models/DictionaryTag.hpp"
#include "mindnet/plugins/dictionary/models/DictionaryTagType.hpp"
#include "mindnet/plugins/dictionary/models/DictionaryTerm.hpp"
#include "mindnet/plugins/dictionary/models/DictionaryTermAlias.hpp"
#include "mindnet/plugins/dictionary/models/DictionaryTermVisit.hpp"
#include "mindnet/plugins/dictionary/models/DictionaryIndexType.hpp"
#include "mindnet/plugins/dictionary/models/DictionaryIndex.hpp"

namespace mindnet::plugins::dictionary::models
{
    struct User;
}

namespace mindnet::plugins::dictionary
{
    bool has_map_name(const api::RequestContext& ctx, string map_name);
    bool has_right_for_map(const api::RequestContext& ctx, const identification map_id,
                           const plugins::core::enums::SingleRight single_right);
    bool has_right_for_map(
        const api::RequestContext& ctx,
        const models::DictionaryNote& dictionary_note,
        const plugins::core::enums::SingleRight single_right);

    gen_find_h(dictionary, DictionaryFlag, dictionary_flag)
    gen_find_h(dictionary, DictionaryLink, dictionary_link)
    gen_find_h(dictionary, DictionaryMap, dictionary_map)
    gen_find_h(dictionary, DictionaryNote, dictionary_note)
    gen_find_h(dictionary, DictionaryReview, dictionary_review)
    gen_find_h(dictionary, DictionarySource, dictionary_source)
    gen_find_h(dictionary, DictionarySourceType, dictionary_source_type)
    gen_find_h(dictionary, DictionaryState18, dictionary_state18)
    gen_find_h(dictionary, DictionaryTag, dictionary_tag)
    gen_find_h(dictionary, DictionaryTagType, dictionary_tag_type)
    gen_find_h(dictionary, DictionaryTerm, dictionary_term)
    gen_find_h(dictionary, DictionaryTermAlias, dictionary_term_alias)
    gen_find_h(dictionary, DictionaryTermVisit, dictionary_term_visit)
    gen_find_h(dictionary, DictionaryIndexType, dictionary_index_type)
    gen_find_h(dictionary, DictionaryIndex, dictionary_index)
}
