//
// Created by robertvokac on 9/6/25.
//

#include "mindnet/persistence/api/PersistenceMethods.h"

#include "mindnet/enums/SingleRight.h"
#include "mindnet/http/QueryParams.h"
#include "mindnet/persistence/api/CrudlValidatorBase.h"

namespace mindnet::persistence::api
{
    std::pair<models::User, api::OperationResult> find_logged_user(
        IPersistence*& db, http::LoginToken token)
    {
        auto result = db->read(models::USER_DEFINITION, token, token.user_id);
        if (result.second.ko())
        {
            return {models::User(), result.second};
        }
        models::User user;
        user.from_values(result.first);
        return {user, ok_result};
    }

    bool has_user_name(const RequestContext& ctx, string user_name)
    {
        string error;
        http::QueryParams query_params;
        query_params.filters.emplace("name", user_name);

        return !ctx.db->list(models::USER_DEFINITION, ctx.token, query_params).first.empty();
    }
    bool has_user_email(const RequestContext& ctx, string user_email)
    {

        string error;
        http::QueryParams query_params;
        query_params.filters.emplace("email", user_email);
        return !ctx.db->list(models::USER_DEFINITION, ctx.token, query_params).first.empty();
    }

    bool has_map_name(const RequestContext& ctx, string map_name)
    {
        string error;
        http::QueryParams query_params;
        query_params.filters.emplace("name", map_name);

        return !ctx.db->list(models::MAP_DEFINITION, ctx.token, query_params).first.empty();
    }

    string is_member_of_team(const RequestContext& ctx, int team_id)
    {
        auto team_result = ctx.db->read(models::TEAM_DEFINITION, ctx.token, team_id);
        if (team_result.second.ko()) return team_result.second.error;
        models::Team team;
        team.from_values(team_result.first);

        http::QueryParams query_params;
        query_params.filters.emplace("team_id", std::to_string(team.get_id()));
        query_params.filters.emplace("user_id", std::to_string(ctx.token.user_id));
        query_params.filters.emplace("status", std::to_string(cast64(enums::UserStatus::ACTIVE)));
        auto is_team_member_result = ctx.db->list(models::TEAM_MEMBER_DEFINITION, ctx.token, query_params);
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
        auto notes = ctx.db->list(models::NOTE_DEFINITION, ctx.token, query_params);
        if (notes.second.ko()) return {-1,notes.second.error};
        if (notes.first.empty()) return {-1, std::string("There is no note with content id") + std::to_string(content_id)};
        models::Note note;
        note.from_values(notes.first.at(0));
        return {note.get_id(), ""};
    }

    bool has_right_for_map(
        const RequestContext& ctx, const int map_id, const enums::SingleRight single_right)
    {
        if (ctx.role == enums::UserRole::ADMIN) { return true; }

        auto map = find_model(map, map_id)
        if (map.second.empty()) return false;

        bool map_owner_and_can = ctx.token.user_id == map.first.owner_id && mindnet::enums::can(
            single_right, map.first.owner_rights);
        if (map_owner_and_can) return true;

        bool map_team_member_and_can = false;

        if (map.first.team_id != 0)
        {
            auto result = is_member_of_team(ctx, map.first.team_id);
            map_team_member_and_can = !result.empty() && mindnet::enums::can(
                enums::SingleRight::WRITE, map.first.team_rights);
            if (map_team_member_and_can) return true;
            bool other_can = mindnet::enums::can(single_right, map.first.other_rights);
            if (other_can) return true;
        }
        return false;
    }

    gen_find_cpp(Comment, comment, COMMENT)
    gen_find_cpp(User, user, USER)
    gen_find_cpp(Message, message, MESSAGE)
    gen_find_cpp(Team, team, TEAM)
    gen_find_cpp(TeamMember, team_member, TEAM_MEMBER)
    gen_find_cpp(Discussion, discussion, DISCUSSION)
    gen_find_cpp(Suggestion, suggestion, SUGGESTION)
    gen_find_cpp(SuggestionReview, suggestion_review, SUGGESTION_REVIEW)
    gen_find_cpp(History, history, HISTORY)
    gen_find_cpp(Map, map, MAP)
    gen_find_cpp(Content, content, CONTENT)
    gen_find_cpp(Note, note, NOTE)
    gen_find_cpp(Property, property, PROPERTY)
    gen_find_cpp(TagType, tag_type, TAG_TYPE)
    gen_find_cpp(Tag, tag, TAG)
    gen_find_cpp(Collection, collection, COLLECTION)
    gen_find_cpp(CollectionItem, collection_item, COLLECTION_ITEM)
    gen_find_cpp(Review, review, REVIEW)
    gen_find_cpp(SM2State, sm2_state, SM2_STATE)
    gen_find_cpp(Question, question, QUESTION)
    gen_find_cpp(Reference, reference, REFERENCE)
    gen_find_cpp(Link, link, LINK)
}
