//
// Created by robertvokac on 8/30/25.
//

#ifndef MIND_NET_CONVERTORS_H
#define MIND_NET_CONVERTORS_H
#include "crow/json.h"
#include "mindnet/Helper.h"

#define start_of_request_to_entity_fields(plugin, Model)\
typedef plugins :: plugin :: columns::Model##Columns cols;\
entity_fields fields;\
\
bool create = crudl == mindnet::plugins::core::enums::Crudl::CREATE;\
bool update = crudl == mindnet::plugins::core::enums::Crudl::UPDATE;\
if (!create && !update)\
{\
return fields;\
}\
\
fields.emplace_back(0);\
\
if (create)\
{\
fields.emplace_back(cast64(Utils::currentUnixTimestamp()));\
}\
else\
{\
fields.emplace_back(cast64(0));\
}\
fields.emplace_back(cast64(Utils::currentUnixTimestamp()));

#define string_for(COLUMN) body[cols::COLUMN].s()
#define int_for(COLUMN) cast64(body[cols::COLUMN])
#define mandatory_string(COLUMN) fields.emplace_back(string_for(COLUMN));
#define mandatory_int(COLUMN) fields.emplace_back(int_for(COLUMN));

#define optional_string(COLUMN, DEFAULT_VALUE) \
if (body.has(cols::COLUMN)) {fields.emplace_back(string_for(COLUMN));}\
else{fields.emplace_back(DEFAULT_VALUE);}

#define optional_int(COLUMN, DEFAULT_VALUE) \
if (body.has(cols::COLUMN)) {fields.emplace_back(int_for(COLUMN));}\
else{fields.emplace_back(DEFAULT_VALUE);}

#define foreign_key(COLUMN)\
if (\
body.has(cols::COLUMN) &&\
body[cols::COLUMN] != 0)\
{\
mandatory_int(COLUMN);\
} else\
{\
fields.emplace_back(FOREIGN_KEY_NULL);\
}

#define method_arguments() crow::json::rvalue& body, mindnet::plugins::core::enums::Crudl crudl

namespace mindnet::impl::sqlite
{
    entity_fields request_to_entity_fields_user(method_arguments());
    entity_fields request_to_entity_fields_message(method_arguments());
    entity_fields request_to_entity_fields_team(method_arguments());
    entity_fields request_to_entity_fields_team_member(method_arguments());
    entity_fields request_to_entity_fields_discussion(method_arguments());
    entity_fields request_to_entity_fields_comment(method_arguments());
    entity_fields request_to_entity_fields_suggestion(method_arguments());
    entity_fields request_to_entity_fields_suggestion_review(method_arguments());
    entity_fields request_to_entity_fields_history(method_arguments());
    entity_fields request_to_entity_fields_map(method_arguments());
    entity_fields request_to_entity_fields_content(method_arguments());
    entity_fields request_to_entity_fields_note(method_arguments());
    entity_fields request_to_entity_fields_property(method_arguments());
    entity_fields request_to_entity_fields_tag_type(method_arguments());
    entity_fields request_to_entity_fields_tag(method_arguments());
    entity_fields request_to_entity_fields_collection(method_arguments());
    entity_fields request_to_entity_fields_collection_item(method_arguments());
    entity_fields request_to_entity_fields_review(method_arguments());
    entity_fields request_to_entity_fields_sm2_state(method_arguments());
    entity_fields request_to_entity_fields_question(method_arguments());
    entity_fields request_to_entity_fields_reference(method_arguments());
    entity_fields request_to_entity_fields_link(method_arguments());
}
#endif //MIND_NET_CONVERTORS_H
