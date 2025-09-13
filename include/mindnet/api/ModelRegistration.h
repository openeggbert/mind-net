//
// Created by robertvokac on 9/13/25.
//

#ifndef MIND_NET_MODELREGISTRATION_H
#define MIND_NET_MODELREGISTRATION_H
#include "IValidator.h"
#include "mindnet/model/ModelDefinition.h"

namespace mindnet::api
{
    struct ModelRegistration
    {
        ModelRegistration(const model::ModelDefinition& model_definition, const std::shared_ptr<IValidator>& validator,
            api::request_to_entity_fields_pointer convert_rest_request_to_entity_fields_pointer,
            const std::shared_ptr<IRepository>& repository)
            : model_definition(model_definition),
              validator(validator),
              convert_rest_request_to_entity_fields_pointer(convert_rest_request_to_entity_fields_pointer),
              repository(repository)
        {
        }

        ~ModelRegistration() = default;
        model::ModelDefinition model_definition;
        std::shared_ptr<IValidator> validator;
        api::request_to_entity_fields_pointer convert_rest_request_to_entity_fields_pointer;
        std::shared_ptr<IRepository> repository;
    };
}
#endif //MIND_NET_MODELREGISTRATION_H
