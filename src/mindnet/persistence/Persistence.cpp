//
// Created by robertvokac on 8/6/25.
//

#include "mindnet/persistence/Persistence.h"

#include "mindnet/persistence/impl/sqlite/repositories/MapRepositoryImplSqlite.h"
#include <memory>

#define add_repository(key, clazz) repositories[#key] = \
    std::make_shared<clazz##RepositoryImplSqlite>(); \
    repositoryNames.push_back(#key);

namespace mindnet::persistence
{
    using namespace mindnet::impl::sqlite::repositories;

    Persistence::Persistence()
    {
        models::IRepository* map_repo = new MapRepositoryImplSqlite();
        repositories["map"] = map_repo;
    }

    Persistence::~Persistence()
    {
        for (auto& e : list_repositories())
        {
            delete get_repository(e);
        }
    }

    models::IRepository* Persistence::get_repository(const std::string& name)
    {
        return repositories.count(name) ? repositories[name] : nullptr;
    }

    std::vector<std::string>& Persistence::list_repositories()
    {
        return repositoryNames;
    }

    int Persistence::create(const models::BaseModel& model)
    {
        return get_repository(model.get_definition().model_name)->create(model);
    }

    entity_fields Persistence::read(int id, models::ModelDefinition& def)
    {
        return get_repository(def.model_name)->read(id, def);
    }
}
