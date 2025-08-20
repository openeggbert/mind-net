//
// Created by robertvokac on 8/16/25.
//
#ifndef CONTENTCONTROLLER_H
#define CONTENTCONTROLLER_H

#include "crow.h"
#include "mindnet/persistence/Persistence.h"

namespace mindnet::routes
{
    class ContentController
    {
    public:
        void register_routes(crow::SimpleApp& app, std::shared_ptr<persistence::Persistence>& db, models::ModelDefinition& def);
    };
}
#endif // CONTENTCONTROLLER_H
