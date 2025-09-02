//
// Created by robertvokac on 8/6/25.
//
#ifndef DB_H
#define DB_H

#include "api/CrudlValidator.h"
#include "api/IRepository.h"
#include "crow/json.h"
#include "mindnet/models/misc/ModelDefinition.h"

namespace mindnet::persistence
{
    using mindnet::models::misc::ModelDefinition;

    using validator = api::CrudlValidator*;
    class Persistence
    {
    private:
        std::map<std::string, api::IRepository*> repositories;
        std::map<std::string, validator> validators;
        std::vector<std::string> repository_names;
        api::IRepository* get_repository(const std::string& name);
        api::CrudlValidator* get_validator(const std::string& name);
        const std::vector<validator> empty_vector;


    public:
        Persistence();
        ~Persistence();

        bool has_repository(const std::string& name);

        std::vector<std::string>& list_repositories();
        string can_create(const ModelDefinition& model_definition, entity_fields& ef);
        string can_read(const ModelDefinition& model_definition, int id);

        int create(const ModelDefinition& def, entity_fields& fields, string& error);
        entity_fields read(int id, const ModelDefinition& def, string& error);
        bool update(int id, entity_fields& fields, ModelDefinition& def, string& error);
        bool remove(int id, ModelDefinition& def, string& error);
        std::optional<ModelDefinition> get_model_definition(string& model_name);

        std::vector<entity_fields> list(http::QueryParams& query_params, ModelDefinition& def, string& error);


        //
        entity_fields request_to_entity_fields(crow::json::rvalue& body, enums::Crudl crudl,
                                                                ModelDefinition& def);
    };
}
#endif // DB_H
