//
// Created by robertvokac on 8/6/25.
//
#ifndef DB_H
#define DB_H

#include "api/ContentRepository.h"
#include "api/IRepository.h"
#include "api/MapRepository.h"



namespace mindnet::persistence {
    class Persistence {
    private:
        std::map<std::string, models::IRepository* > repositories;
        std::vector<std::string> repositoryNames;



    public:
        Persistence();
        ~Persistence();

        models::IRepository* get_repository(const std::string& name);

        std::vector<std::string>& list_repositories();

        int create(const models::ModelDefinition& def, entity_fields& fields);
        entity_fields read(int id, models::ModelDefinition& def);
        bool update(int id, entity_fields& fields, models::ModelDefinition& def);
        bool remove(int id, models::ModelDefinition& def);

        std::vector<entity_fields> list(size_t page_number, size_t pageSize, models::ModelDefinition& def);

        //
        entity_fields convert_crow_json_rvalue_to_entity_fields(crow::json::rvalue& body, models::ModelDefinition& def);


    };

}
#endif // DB_H
