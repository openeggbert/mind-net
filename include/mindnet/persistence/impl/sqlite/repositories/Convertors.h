//
// Created by robertvokac on 8/30/25.
//

#ifndef MIND_NET_CONVERTORS_H
#define MIND_NET_CONVERTORS_H
#include "crow/json.h"
#include "mindnet/Helper.h"
#include "mindnet/enums/Crudl.h"

#define method_arguments() crow::json::rvalue& body, enums::Crudl crudl
namespace mindnet::persistence::impl::sqlite::repositories
{
    entity_fields convert_crow_json_rvalue_to_entity_fields_user(method_arguments());
    entity_fields convert_crow_json_rvalue_to_entity_fields_history(method_arguments());
    entity_fields convert_crow_json_rvalue_to_entity_fields_map(method_arguments());
    entity_fields convert_crow_json_rvalue_to_entity_fields_note(method_arguments());
    entity_fields convert_crow_json_rvalue_to_entity_fields_content(method_arguments());
    entity_fields convert_crow_json_rvalue_to_entity_fields_property(method_arguments());
    entity_fields convert_crow_json_rvalue_to_entity_fields_tag_type(method_arguments());
    entity_fields convert_crow_json_rvalue_to_entity_fields_tag(method_arguments());
    entity_fields convert_crow_json_rvalue_to_entity_fields_reference(method_arguments());
    entity_fields convert_crow_json_rvalue_to_entity_fields_link(method_arguments());


}
#endif //MIND_NET_CONVERTORS_H