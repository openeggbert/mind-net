//
// Created by robertvokac on 8/16/25.
//

#ifndef CONTENTROUTES_H
#define CONTENTROUTES_H

#include "crow.h"

namespace miniwiki::routes {

    class ContentRoutes {
    public:
        static void register_routes(crow::SimpleApp& app);
    };

}


#endif //CONTENTROUTES_H