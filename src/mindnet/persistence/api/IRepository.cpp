//
// Created by robertvokac on 8/30/25.
//
// IRepository.cpp
#include "mindnet/persistence/api/IRepository.h"

namespace mindnet::persistence::api
{
    IRepository::IRepository(
        api::request_to_entity_fields_pointer convert_rest_request_to_entity_fields_pointer_,
        models::misc::ModelDefinition& model_definition_
    ) :
        request_to_entity_fields_pointer_(convert_rest_request_to_entity_fields_pointer_),
        model_definition(model_definition_)
    {
        if (convert_rest_request_to_entity_fields_pointer_ == nullptr)
        {
            throw std::invalid_argument("convert_rest_request_to_entity_fields_pointer cannot be null");
        }
    } ;
}
