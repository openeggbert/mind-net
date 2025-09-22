//
// Created by robertvokac on 9/13/25.
//

#ifndef MIND_NET_MODELREGISTRATION_H
#define MIND_NET_MODELREGISTRATION_H
#include <utility>
#include <memory>
#include "mindnet/other/api/IValidator.h"
#include "mindnet/other/model/ModelDefinition.h"

namespace mindnet::api
{
    class IRepository;

    struct ModelRegistration
    {
        ModelRegistration(
            model::ModelDefinition model_definition,
            const std::shared_ptr<IValidator>& validator,
            const std::shared_ptr<IRepository>& repository)
            : model_definition(std::move(model_definition)),
              validator(validator),
              repository(repository)
        {
        }

        ~ModelRegistration() = default;
        model::ModelDefinition model_definition;
        std::shared_ptr<IValidator> validator;
        std::shared_ptr<IRepository> repository;
    };
}
#endif //MIND_NET_MODELREGISTRATION_H
