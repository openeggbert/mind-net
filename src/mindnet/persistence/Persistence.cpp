//
// Created by robertvokac on 8/6/25.
//

#include "mindnet/persistence/Persistence.h"

#include "mindnet/persistence/impl/sqlite/repositories/MapRepositoryImplSqlite.h"
#include "mindnet/persistence/impl/sqlite/repositories/ContentRepositoryImplSqlite.h"
#include "mindnet/persistence/impl/sqlite/repositories/HistoryRepositoryImplSqlite.h"
#include "mindnet/persistence/impl/sqlite/repositories/NodeRepositoryImplSqlite.h"
#include "mindnet/persistence/impl/sqlite/repositories/NodePropertyRepositoryImplSqlite.h"
#include "mindnet/persistence/impl/sqlite/repositories/TagRepositoryImplSqlite.h"
#include "mindnet/persistence/impl/sqlite/repositories/NodeTagRepositoryImplSqlite.h"
#include "mindnet/persistence/impl/sqlite/repositories/NodeLinkRepositoryImplSqlite.h"
#include "mindnet/persistence/impl/sqlite/repositories/ExternalLinkRepositoryImplSqlite.h"
#include <memory>


#define add_repository(model, Model) \
models::IRepository* model##_repo = new Model##RepositoryImplSqlite();\
repositories[#model] = model##_repo;

namespace mindnet::persistence
{
    using namespace mindnet::impl::sqlite::repositories;

    Persistence::Persistence()
    {
        add_repository(history, History);
        add_repository(map, Map);
        add_repository(node, Node);
        add_repository(content, Content);
        add_repository(node_property, NodeProperty);
        //
        add_repository(tag, Tag);
        add_repository(node_tag, NodeTag);
        add_repository(node_link, NodeLink);
        add_repository(external_link, ExternalLink);
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

    int Persistence::create(const models::misc::ModelDefinition& def, entity_fields& fields, str& error)
    {
        return get_repository(def.model_name)->create(fields, error);
    }

    entity_fields Persistence::read(const int id, const models::misc::ModelDefinition& def)
    {
        return get_repository(def.model_name)->read(id);
    }

    bool Persistence::update(int id, entity_fields& fields, models::misc::ModelDefinition& def)
    {
        return get_repository(def.model_name)->update(id, fields);
    }

    bool Persistence::remove(int id, models::misc::ModelDefinition& def)
    {
        return get_repository(def.model_name)->remove(id);
    }

    std::vector<entity_fields> Persistence::list(size_t page_number, size_t pageSize, int& total_items, models::misc::ModelDefinition& def)
    {
        return get_repository(def.model_name)->list(page_number, pageSize, total_items);
    }

    entity_fields Persistence::convert_crow_json_rvalue_to_entity_fields(
        crow::json::rvalue& body, const enums::Crudl crudl, models::misc::ModelDefinition& def)
    {
        return get_repository(def.model_name)->convert_crow_json_rvalue_to_entity_fields(body, crudl);
    }
}
