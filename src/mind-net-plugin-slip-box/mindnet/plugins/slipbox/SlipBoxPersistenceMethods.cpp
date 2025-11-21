//
// Created by robertvokac on 9/6/25.
//

#include "mindnet/plugins/slipbox/SlipBoxPersistenceMethods.hpp"

#include "mindnet/api/IService.hpp"
#include "mindnet/plugins/core/enums/SingleRight.hpp"
#include "mindnet/orm/QueryParams.hpp"
#include "mindnet/api/ValidatorBase.hpp"
#include "mindnet/plugins/core/models/Team.hpp"
#include "mindnet/plugins/core/models/User.hpp"
#include "mindnet/plugins/slipbox/models/MapCollection.hpp"
#include "mindnet/plugins/slipbox/models/Url.hpp"

namespace mindnet::plugins::slipbox
{
    bool has_map_name(const api::RequestContext& ctx, string map_name)
    {
        string error;
        orm::QueryParams query_params;
        query_params.filters.emplace("name", map_name);

        return !ctx.db->list(plugins::slipbox::models::MAP_DEFINITION, ctx.token, query_params).first.empty();
    }


    std::pair<int, string> find_note_for_content(const api::RequestContext& ctx, int content_id)
    {
        orm::QueryParams query_params;
        query_params.filters.emplace("content_id", std::to_string(content_id));
        auto notes = ctx.db->list(plugins::slipbox::models::NOTE_DEFINITION, ctx.token, query_params);
        if (notes.second.ko()) return {-1, notes.second.error};
        if (notes.first.empty())
            return {
                -1, std::string("There is no note with content id ") + std::to_string(content_id)
            };
        plugins::slipbox::models::Note note;
        note.from_values(notes.first.at(0));
        return {note.get_id(), ""};
    }

    bool has_right_for_map(
        const api::RequestContext& ctx, const int map_id, const plugins::core::enums::SingleRight single_right)
    {
        if (ctx.role == essential::UserRole::Admin) { return true; }

        auto map = slipbox::find_map(ctx, map_id);
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

    gen_find_cpp(core, User, user, USER)
    // gen_find_cpp(mail, Message, message, MESSAGE)
    gen_find_cpp(core, Team, team, TEAM)
    gen_find_cpp(core, TeamMember, team_member, TEAM_MEMBER)
    // gen_find_cpp(core, History, history, HISTORY)
    gen_find_cpp(slipbox, Map, map, MAP)
    gen_find_cpp(slipbox, Content, content, CONTENT)
    gen_find_cpp(slipbox, Note, note, NOTE)
    gen_find_cpp(slipbox, Test, test, TEST)
    gen_find_cpp(slipbox, TestAttempt, test_attempt, TEST_ATTEMPT)
    gen_find_cpp(slipbox, Property, property, PROPERTY)
    gen_find_cpp(slipbox, TagType, tag_type, TAG_TYPE)
    gen_find_cpp(slipbox, Tag, tag, TAG)
    gen_find_cpp(slipbox, Collection, collection, COLLECTION)
    gen_find_cpp(slipbox, CollectionItem, collection_item, COLLECTION_ITEM)
    // gen_find_cpp(repetition, R2Review, r2_review, R2_REVIEW)
    // gen_find_cpp(repetition, R2State, r2_state, R2_STATE)
    gen_find_cpp(slipbox, Question, question, QUESTION)
    gen_find_cpp(slipbox, Link, link, LINK)
    gen_find_cpp(slipbox, Url, url, URL)
    gen_find_cpp(slipbox, MapCollection, map_collection, MAP_COLLECTION)
}
