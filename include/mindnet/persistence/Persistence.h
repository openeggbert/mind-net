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

        int create(const models::BaseModel& content);
        entity_fields read(int id, models::ModelDefinition& def);

    };

}
#endif // DB_H
