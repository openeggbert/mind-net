//
// Created by robertvokac on 8/6/25.
//
#ifndef DB_H
#define DB_H

#include "api/IRepository.h"
#include "crow/json.h"
#include "mindnet/models/misc/ModelDefinition.h"

namespace mindnet::persistence
{
    using mindnet::models::misc::ModelDefinition;

    class Persistence
    {
    private:
        std::map<std::string, api::IRepository*> repositories;
        std::vector<std::string> repositoryNames;

    public:
        Persistence();
        ~Persistence();

        api::IRepository* get_repository(const std::string& name);

        std::vector<std::string>& list_repositories();

        int create(const ModelDefinition& def, entity_fields& fields, string& error);
        entity_fields read(int id, const ModelDefinition& def, string& error);
        bool update(int id, entity_fields& fields, ModelDefinition& def, string& error);
        bool remove(int id, ModelDefinition& def, string& error);

        std::vector<entity_fields> list(http::QueryParams& query_params, ModelDefinition& def, string& error);

        //
        entity_fields convert_crow_json_rvalue_to_entity_fields(crow::json::rvalue& body, enums::Crudl crudl,
                                                                ModelDefinition& def);
    };
}
#endif // DB_H
