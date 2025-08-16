//
// Created by robertvokac on 8/16/25.
//
#ifndef CONTENTCONTROLLER_H
#define CONTENTCONTROLLER_H

#include "crow.h"
#include "miniwiki/persistence/Persistence.h"

namespace miniwiki::routes
{
    class ContentController
    {
    public:
        void register_routes(crow::SimpleApp& app, std::shared_ptr<persistence::Persistence> db);
    };
}
#endif // CONTENTCONTROLLER_H
