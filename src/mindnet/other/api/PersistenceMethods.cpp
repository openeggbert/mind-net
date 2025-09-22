//
// Created by robertvokac on 9/6/25.
//

#include "mindnet/other/api/PersistenceMethods.h"

#include "mindnet/other/plugins/core/enums/SingleRight.h"
#include "mindnet/other/http/QueryParams.h"
#include "mindnet/other/api/ValidatorBase.h"

namespace mindnet::api
{
    std::pair<plugins::core::models::User, mindnet::api::OperationResult> find_logged_user(
        DbPtr& db, http::LoginToken token)
    {
        if (token.user_id == 0)
        {
            plugins::core::models::User u;
            u.role = plugins::core::enums::UserRole::Guest;
            u.status = plugins::core::enums::UserStatus::Active;
            return {u, ok_result};
        }
        auto result = db->read(plugins::core::models::USER_DEFINITION, token, token.user_id);
        if (result.second.ko())
        {
            return {plugins::core::models::User(), result.second};
        }
        plugins::core::models::User user;
        user.from_values(result.first);
        return {user, ok_result};
    }

    bool has_user_name(const RequestContext& ctx, string user_name)
    {
        string error;
        http::QueryParams query_params;
        query_params.filters.emplace("name", user_name);

        return !ctx.db->list(plugins::core::models::USER_DEFINITION, ctx.token, query_params).first.empty();
    }

    bool has_user_email(const RequestContext& ctx, string user_email)
    {
        string error;
        http::QueryParams query_params;
        query_params.filters.emplace("email", user_email);
        return !ctx.db->list(plugins::core::models::USER_DEFINITION, ctx.token, query_params).first.empty();
    }

    bool has_map_name(const RequestContext& ctx, string map_name)
    {
        string error;
        http::QueryParams query_params;
        query_params.filters.emplace("name", map_name);

        return !ctx.db->list(plugins::slipbox::models::MAP_DEFINITION, ctx.token, query_params).first.empty();
    }

    string is_member_of_team(const RequestContext& ctx, int team_id)
    {
        auto team_result = ctx.db->read(plugins::core::models::TEAM_DEFINITION, ctx.token, team_id);
        if (team_result.second.ko()) return team_result.second.error;
        plugins::core::models::Team team;
        team.from_values(team_result.first);

        http::QueryParams query_params;
        query_params.filters.emplace("team_id", std::to_string(team.get_id()));
        query_params.filters.emplace("user_id", std::to_string(ctx.token.user_id));
        query_params.filters.emplace("status", std::to_string(cast64(plugins::core::enums::UserStatus::Active)));
        auto is_team_member_result = ctx.db->list(plugins::core::models::TEAM_MEMBER_DEFINITION, ctx.token,
                                                  query_params);
        if (is_team_member_result.second.ko()) return is_team_member_result.second.error;
        if (is_team_member_result.first.empty())
        {
            return "User is not member of team with id " + std::to_string(team.get_id()) + ".";
        }
        return "";
    }

    std::pair<int, string> find_note_for_content(const RequestContext& ctx, int content_id)
    {
        http::QueryParams query_params;
        query_params.filters.emplace("content_id", std::to_string(content_id));
        auto notes = ctx.db->list(plugins::slipbox::models::NOTE_DEFINITION, ctx.token, query_params);
        if (notes.second.ko()) return {-1, notes.second.error};
        if (notes.first.empty()) return {
            -1, std::string("There is no note with content id") + std::to_string(content_id)
        };
        plugins::slipbox::models::Note note;
        note.from_values(notes.first.at(0));
        return {note.get_id(), ""};
    }

    bool has_right_for_map(
        const RequestContext& ctx, const int map_id, const plugins::core::enums::SingleRight single_right)
    {
        if (ctx.role == plugins::core::enums::UserRole::Admin) { return true; }

        auto map = find_model(map, map_id)
        if (!map.second.empty()) return false;

        bool map_owner_and_can = ctx.token.user_id == map.first.owner_id && can(
            single_right, map.first.owner_rights_int());
        if (map_owner_and_can) return true;

        bool map_team_member_and_can = false;

        if (map.first.team_id != 0)
        {
            auto result = is_member_of_team(ctx, map.first.team_id);
            map_team_member_and_can = !result.empty() && can(
                plugins::core::enums::SingleRight::Write, map.first.team_rights_int());
            if (map_team_member_and_can) return true;
        }

        bool other_can = can(single_right, map.first.other_rights_int());
        if (other_can) return true;

        return false;
    }

    gen_find_cpp(chat, Comment, comment, COMMENT)
    gen_find_cpp(core, User, user, USER)
    gen_find_cpp(mail, Message, message, MESSAGE)
    gen_find_cpp(core, Team, team, TEAM)
    gen_find_cpp(core, TeamMember, team_member, TEAM_MEMBER)
    gen_find_cpp(chat, Discussion, discussion, DISCUSSION)
    gen_find_cpp(suggestion, Suggestion, suggestion, SUGGESTION)
    gen_find_cpp(suggestion, SuggestionReview, suggestion_review, SUGGESTION_REVIEW)
    gen_find_cpp(core, History, history, HISTORY)
    gen_find_cpp(slipbox, Map, map, MAP)
    gen_find_cpp(slipbox, Content, content, CONTENT)
    gen_find_cpp(slipbox, Note, note, NOTE)
    gen_find_cpp(slipbox, Property, property, PROPERTY)
    gen_find_cpp(slipbox, TagType, tag_type, TAG_TYPE)
    gen_find_cpp(slipbox, Tag, tag, TAG)
    gen_find_cpp(slipbox, Collection, collection, COLLECTION)
    gen_find_cpp(slipbox, CollectionItem, collection_item, COLLECTION_ITEM)
    gen_find_cpp(supermemo, Review, review, REVIEW)
    gen_find_cpp(supermemo, SM2State, sm2_state, SM2_STATE)
    gen_find_cpp(slipbox, Question, question, QUESTION)
    gen_find_cpp(slipbox, Reference, reference, REFERENCE)
    gen_find_cpp(slipbox, Link, link, LINK)
}
