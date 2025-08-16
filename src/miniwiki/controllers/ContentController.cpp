//
// Created by robertvokac on 8/16/25.
//

#include "miniwiki/controllers/ContentController.h"
#include "crow.h"

namespace miniwiki::routes {

    void ContentController::register_routes(crow::SimpleApp& app) {
        CROW_ROUTE(app, "/content/<int>")
        ([](int id){
            return crow::response("Content ID: " + std::to_string(id));
        });
    }

}
