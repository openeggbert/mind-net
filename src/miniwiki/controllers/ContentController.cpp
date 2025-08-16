//
// Created by robertvokac on 8/16/25.
//

#include "miniwiki/controllers/ContentController.h"
#include "crow.h"
#include "miniwiki/models/Content.h"

namespace miniwiki::routes
{
    void ContentController::register_routes(crow::SimpleApp& app, std::shared_ptr<persistence::Persistence> db)
    {
        typedef models::columns::ContentColumns c;
        CROW_ROUTE(app, "/content/<int>")
        ([](int id)
        {
            return crow::response("Content ID: " + std::to_string(id));
        });

        CROW_ROUTE(app, "/content").methods(crow::HTTPMethod::POST)
        ([db](const crow::request& req)
        {
            auto body = crow::json::load(req.body);
            if (!body || !body.has(c::CONTENT) || !body.has(c::FORMAT))
                return crow::response(400, "Invalid input");

            models::Content c{0, body[c::CONTENT].s(), body[c::FORMAT].s(),0};
            db.get()->content_repository->create(c);

            crow::json::wvalue res;
            res["id"] = c.id;
            res["content"] = c.content;
            res["format"] = c.format;
            res["created_at"] = c.created_at;
            return crow::response(201, res);
        });
    }
}
