//
// Created by robertvokac on 8/30/25.
//
// IRepository.cpp
#include "mindnet/api/IRepository.h"

namespace mindnet::api
{
    IRepository::IRepository(
        model::ModelDefinition& model_definition_
    ) :
        model_definition(model_definition_)
    {
    }
}
