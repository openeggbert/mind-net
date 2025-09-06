//
// Created by robertvokac on 9/6/25.
//

#include "mindnet/persistence/api/PersistenceMethods.h"

namespace mindnet::persistence::api
{


    bool has_user_name(db_ptr& db, http::LoginToken& token, string user_name)
    {
        string error;
        http::QueryParams query_params;
        query_params.filters.emplace("name", user_name);

        return !db->list(query_params, models::USER_DEFINITION, token).first.empty();
    }
    bool has_user_email(db_ptr& db, http::LoginToken& token, string user_email)
    {

        string error;
        http::QueryParams query_params;
        query_params.filters.emplace("email", user_email);
        return !db->list(query_params, models::USER_DEFINITION, token).first.empty();
    }


    string is_member_of_team(db_ptr& d, http::LoginToken& token, int team_id)
    {
        auto team_result = d->read(team_id, models::TEAM_DEFINITION, token);
        if (team_result.second.ko()) return team_result.second.error;
        models::Team team;
        team.from_values(team_result.first);

        http::QueryParams query_params;
        query_params.filters.emplace("team_id", std::to_string(team.get_id()));
        query_params.filters.emplace("user_id", std::to_string(token.user_id));
        query_params.filters.emplace("status", std::to_string(cast64(enums::UserStatus::ACTIVE)));
        auto is_team_member_result = d->list(query_params, models::TEAM_MEMBER_DEFINITION, token);
        if (is_team_member_result.second.ko()) return is_team_member_result.second.error;
        if (is_team_member_result.first.empty())
        {
            return "User is not member of team with id " + std::to_string(team.get_id()) + ".";
        }
        return "";
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
