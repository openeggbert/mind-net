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

#include "mindnet/plugins/dictionary/DictionaryPersistenceMethods.hpp"

#include "mindnet/api/IService.hpp"
#include "mindnet/plugins/core/enums/SingleRight.hpp"
#include "mindnet/orm/QueryParams.hpp"
#include "mindnet/api/ValidatorBase.hpp"
#include "mindnet/plugins/core/models/Team.hpp"
#include "mindnet/plugins/core/models/User.hpp"

namespace mindnet::plugins::dictionary
{
    bool has_map_name(const api::RequestContext& ctx, string map_name)
    {
        string error;
        orm::QueryParams query_params;
        query_params.filters.emplace("name", map_name);

        return !ctx.db->list(plugins::dictionary::models::DICTIONARY_MAP_DEFINITION, ctx.token, query_params).first.
                    empty();
    }

    bool has_right_for_map(
        const api::RequestContext& ctx, const identification map_id,
        const plugins::core::enums::SingleRight single_right)
    {
        if (ctx.role == essential::UserRole::Admin) { return true; }

        auto map = dictionary::find_dictionary_map(ctx, map_id);
        if (!map.second.empty()) return false;

        bool map_owner_and_can = ctx.token.user_id == map.first.owner_id && can(
            single_right, map.first.owner_rights_int());
        if (map_owner_and_can) return true;

        bool map_team_member_and_can = false;

        if (map.first.team_id != 0)
        {
            auto result = core::is_member_of_team(ctx, map.first.team_id);
            map_team_member_and_can = !result.empty() && can(
                plugins::core::enums::SingleRight::Write, map.first.team_rights_int());
            if (map_team_member_and_can) return true;
        }

        bool other_can = can(single_right, map.first.other_rights_int());
        if (other_can) return true;

        return false;
    }
    bool has_right_for_map(
        const api::RequestContext& ctx,
        const models::DictionaryNote& dictionary_note,
        const plugins::core::enums::SingleRight single_right)
    {
        auto dictionary_term_id = dictionary_note.dictionary_term_id;
        auto dictionary_term = find_dictionary_term(ctx, dictionary_term_id);
        if (!dictionary_term.second.empty()) { return false; }

        return has_right_for_map(ctx, dictionary_term.first.dictionary_map_id, single_right);
    }
    gen_find_cpp(dictionary, DictionaryMap, dictionary_map, DICTIONARY_MAP)
    gen_find_cpp(dictionary, DictionaryNote, dictionary_note, DICTIONARY_NOTE)
    gen_find_cpp(dictionary, DictionaryTagType, dictionary_tag_type, DICTIONARY_TAG_TYPE)
    gen_find_cpp(dictionary, DictionaryTag, dictionary_tag, DICTIONARY_TAG)
    gen_find_cpp(dictionary, DictionaryLink, dictionary_link, DICTIONARY_LINK)
    gen_find_cpp(dictionary, DictionaryTerm, dictionary_term, DICTIONARY_TERM)
    gen_find_cpp(dictionary, DictionaryTermVisit, dictionary_term_visit, DICTIONARY_TERM_VISIT)
}
