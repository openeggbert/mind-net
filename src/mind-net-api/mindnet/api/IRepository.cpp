//
// Created by robertvokac on 8/30/25.
//
// IRepository.cpp
#include "mindnet/api/IRepository.hpp"

namespace mindnet::api
{
    IRepository::IRepository(
        const model::ModelDefinition& model_definition_
    ) :
        model_definition(model_definition_)
    {
    }
}
