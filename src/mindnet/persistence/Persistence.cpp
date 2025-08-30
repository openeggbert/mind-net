//
// Created by robertvokac on 8/6/25.
//

#include "mindnet/persistence/Persistence.h"

#include "mindnet/models/User.h"
#include "mindnet/models/History.h"
#include "mindnet/models/Map.h"
#include "mindnet/models/Note.h"
#include "mindnet/models/Content.h"
#include "mindnet/models/Property.h"
#include "mindnet/models/TagType.h"
#include "mindnet/models/Tag.h"
#include "mindnet/models/Reference.h"
#include "mindnet/models/Link.h"

#include "mindnet/persistence/impl/sqlite/repositories/Convertors.h"
#include "mindnet/persistence/impl/sqlite/repositories/RepositoryImplSqlite.h"

#define MODEL_JOIN(x) x##_DEFINITION
#define FUNCTION_JOIN(x) convert_crow_json_rvalue_to_entity_fields_##x

#define add_repository(model, Model, MODEL) \
models::IRepository* model##_repo = new RepositoryImplSqlite(\
& FUNCTION_JOIN(model),\
models :: MODEL_JOIN(MODEL)\
);\
repositories[#model] = model##_repo;\
repositoryNames.push_back(#model);

namespace mindnet::persistence
{
    using namespace mindnet::persistence::impl::sqlite::repositories;

    Persistence::Persistence()
    {
        // models::IRepository* user_repo =
        //     new RepositoryImplSqlite(
        //         &convert_crow_json_rvalue_to_entity_fields_user,
        //         models::USER_DEFINITION
        //         );
        // repositories["user"] = user_repo;;

        add_repository(user, User, USER);
        add_repository(history, History, HISTORY);
        add_repository(map, Map, MAP);
        add_repository(note, Note, NOTE);
        add_repository(content, Content, CONTENT);
        add_repository(property, Property, PROPERTY);
        //
        add_repository(tag_type, TagType, TAG_TYPE);
        add_repository(tag, Tag, TAG);
        add_repository(reference, Reference, REFERENCE);
        add_repository(link, Link, LINK);
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
