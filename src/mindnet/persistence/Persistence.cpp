//
// Created by robertvokac on 8/6/25.
//

#include "mindnet/persistence/Persistence.h"
#include "mindnet/persistence/impl/sqlite/repositories/UserRepositoryImplSqlite.h"
#include "mindnet/persistence/impl/sqlite/repositories/MapRepositoryImplSqlite.h"
#include "mindnet/persistence/impl/sqlite/repositories/ContentRepositoryImplSqlite.h"
#include "mindnet/persistence/impl/sqlite/repositories/HistoryRepositoryImplSqlite.h"
#include "mindnet/persistence/impl/sqlite/repositories/NoteRepositoryImplSqlite.h"
#include "mindnet/persistence/impl/sqlite/repositories/PropertyRepositoryImplSqlite.h"
#include "mindnet/persistence/impl/sqlite/repositories/TagTypeRepositoryImplSqlite.h"
#include "mindnet/persistence/impl/sqlite/repositories/TagRepositoryImplSqlite.h"
#include "mindnet/persistence/impl/sqlite/repositories/ReferenceRepositoryImplSqlite.h"
#include "mindnet/persistence/impl/sqlite/repositories/LinkRepositoryImplSqlite.h"

#define add_repository(model, Model) \
models::IRepository* model##_repo = new Model##RepositoryImplSqlite();\
repositories[#model] = model##_repo;

namespace mindnet::persistence
{
    using namespace mindnet::impl::sqlite::repositories;

    Persistence::Persistence()
    {

        add_repository(user, User);
        add_repository(history, History);
        add_repository(map, Map);
        add_repository(note, Note);
        add_repository(content, Content);
        add_repository(property, Property);
        //
        add_repository(tag_type, TagType);
        add_repository(tag, Tag);
        add_repository(reference, Reference);
        add_repository(link, Link);
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

    int Persistence::create(const models::misc::ModelDefinition& def, entity_fields& fields, string& error)
    {
        return get_repository(def.get_model_name())->create(fields, error);
    }

    entity_fields Persistence::read(const int id, const models::misc::ModelDefinition& def, string& error)
    {
        return get_repository(def.get_model_name())->read(id, error);
    }

    bool Persistence::update(int id, entity_fields& fields, models::misc::ModelDefinition& def, string& error)
    {
        return get_repository(def.get_model_name())->update(id, fields, error);
    }

    bool Persistence::remove(int id, models::misc::ModelDefinition& def, string& error)
    {
        return get_repository(def.get_model_name())->remove(id, error);
    }

    std::vector<entity_fields> Persistence::list(http::QueryParams& query_params, ModelDefinition& def, string& error)
    {
        return get_repository(def.get_model_name())->list(query_params, error);
    }

    entity_fields Persistence::convert_crow_json_rvalue_to_entity_fields(
        crow::json::rvalue& body, const enums::Crudl crudl, models::misc::ModelDefinition& def)
    {
        return get_repository(def.get_model_name())->convert_crow_json_rvalue_to_entity_fields(body, crudl);
    }
}
