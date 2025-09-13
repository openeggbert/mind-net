//
// Created by robertvokac on 8/16/25.
//
#ifndef MAPCONTROLLER_H
#define MAPCONTROLLER_H


#include "crow.h"
#include "../persistence/api/Persistence.h"

namespace mindnet::routes
{
    class ModelController
    {
    public:
        void register_routes(crow::SimpleApp& app, persistence::api::DbPtr& db,
                             models::misc::ModelDefinition& def);
    };
}
#endif // MAPCONTROLLER_H
