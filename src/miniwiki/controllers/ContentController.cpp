//
// Created by robertvokac on 8/16/25.
//

#include "miniwiki/controllers/ContentController.h"
#include "crow.h"
#include "miniwiki/Utils.h"
#include "miniwiki/models/Content.h"

namespace miniwiki::routes
{
    void ContentController::register_routes(crow::SimpleApp& app, std::shared_ptr<persistence::Persistence>& db)
    {
        typedef models::columns::ContentColumns c;
        CROW_ROUTE(app, "/content/<int>")
        ([db](int id)
        {
            models::Content c;
            try {
            c = db->content_repository->read(id);
            } catch (std::runtime_error& e) {return crow::response(404, "The content with id " + std::to_string(id) + " was not found.");}

            crow::json::wvalue res;
            res["id"] = c.id;
            res["content"] = c.content;
            res["format"] = c.format;
            res["created_at"] = c.created_at;
            return crow::response(201, res);

        });

        CROW_ROUTE(app, "/content").methods(crow::HTTPMethod::POST)
        ([db](const crow::request& req)
        {
            auto body = crow::json::load(req.body);
            if (!body || !body.has(c::CONTENT) || !body.has(c::FORMAT))
                return crow::response(400, "Invalid input");

            models::Content c{0, body[c::CONTENT].s(), body[c::FORMAT].s(),static_cast<int>(Utils::currentUnixTimestamp())};
            auto last_inserted_id = db.get()->content_repository->create(c);

            crow::json::wvalue res;
            res["id"] = last_inserted_id;
            res["content"] = c.content;
            res["format"] = c.format;
            res["created_at"] = c.created_at;
            return crow::response(201, res);
        });
    }
}
