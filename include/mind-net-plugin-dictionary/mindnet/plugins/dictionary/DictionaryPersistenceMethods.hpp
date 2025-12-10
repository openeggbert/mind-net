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
#include "mindnet/plugins/dictionary/models/DictionaryMap.hpp"
#include "mindnet/plugins/dictionary/models/DictionaryNote.hpp"
#include "mindnet/plugins/dictionary/models/DictionaryTagType.hpp"
#include "mindnet/plugins/dictionary/models/DictionaryTag.hpp"
#include "mindnet/plugins/dictionary/models/DictionaryLink.hpp"
#include "mindnet/plugins/dictionary/models/DictionaryTerm.hpp"
#include "mindnet/plugins/dictionary/models/DictionaryTermVisit.hpp"

namespace mindnet::plugins::dictionary::models
{
    struct User;
}

namespace mindnet::plugins::dictionary
{
    bool has_map_name(const api::RequestContext& ctx, string map_name);
    bool has_right_for_map(const api::RequestContext& ctx, const identification map_id,
                           const plugins::core::enums::SingleRight single_right);

    gen_find_h(dictionary, DictionaryMap, map)
    gen_find_h(dictionary, DictionaryNote, note)
    gen_find_h(dictionary, DictionaryTagType, tag_type)
    gen_find_h(dictionary, DictionaryTag, tag)
    gen_find_h(dictionary, DictionaryLink, link)
    gen_find_h(dictionary, DictionaryTerm, term)
    gen_find_h(dictionary, DictionaryTermVisit, term_visit)
}
