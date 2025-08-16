//
// Created by robertvokac on 8/16/25.
//
#ifndef CONTENTCONTROLLER_H
#define CONTENTCONTROLLER_H


#include "crow.h"

namespace miniwiki::routes {

    class ContentController {
    public:
        void register_routes(crow::SimpleApp& app);
    };

}
#endif // CONTENTCONTROLLER_H
