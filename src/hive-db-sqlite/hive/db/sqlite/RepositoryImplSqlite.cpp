/*
 * MIT License
 * Copyright (c) 2025 Robert Vokac
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

/**
 *
 * @author <a href="mailto:robertvokac@robertvokac.com">Robert Vokac</a>
 */

#include "hive/db/sqlite/RepositoryImplSqlite.hpp"

#include "hive/api/IRepository.hpp"
#include "hive/essential/Global.hpp"
#include "hive/db/sqlite/RepositoryUtils.hpp"
#include "hive/util/Utils.hpp"
#include "SQLiteCpp/Database.h"

namespace hive::api
{
    class IRepository;
}

namespace hive::model
{
    struct ModelDefinition;
}

namespace hive::db::sqlite
{
    using_loggers()

    RepositoryImplSqlite::RepositoryImplSqlite(
        const model::ModelDefinition& model_definition_
    ) : api::IRepository(model_definition_)
    {
    }

    RepositoryImplSqlite::~RepositoryImplSqlite() = default;

    identification RepositoryImplSqlite::create(const entity_fields& fields, string& error)
    {
        try { return create_model(get_model_definition(), fields, error); }
        catch (std::exception& e) { return -1; }
    }

    entity_fields RepositoryImplSqlite::read(const identification id, string& error)
    {
        return read_model(get_model_definition(), id, error);
    }

    bool RepositoryImplSqlite::update(identification id, entity_fields& fields, string& error)
    {
        return update_model(get_model_definition(), fields, id, error);
    }

    bool RepositoryImplSqlite::remove(identification id, string& error)
    {
        return delete_model(get_model_definition(), id, error);
    }

    std::vector<entity_fields> RepositoryImplSqlite::list(orm::QueryParams& query_params, string& error)
    {
        return list_models(get_model_definition(), query_params, error);
    }

    std::vector<entity_fields> RepositoryImplSqlite::list_in_ids(std::vector<identification>& ids, string& error)
    {
        orm::QueryParams query_params;
        std::string ids_string;
        for (auto& id : ids)
        {
            query_params.ids.push_back(id);
        }
        return list_models(get_model_definition(), query_params, error, orm::IN_IDS);
    }

    std::vector<identification> RepositoryImplSqlite::list_ids(orm::QueryParams& query_params, string& error)
    {
        std::vector<entity_fields> fields = list_models(get_model_definition(), query_params, error, orm::IDS);
        std::vector<identification> result;
        if (!error.empty()) return result;

        for (auto& f : fields)
        {
            std::variant<string, i64>& i_1 = f[0];
            result.push_back(std::get<i64>(i_1));
        }
        return result;
    }

    model::ModelDefinition& RepositoryImplSqlite::get_model_definition()
    {
        return model_definition;
    }

    //
    // class NumberOrText
    // {
    //     int64_t number{};
    //     std::string text;
    //     crow::json::rvalue rvalue;
    //
    // public:
    //     NumberOrText(
    //         crow::json::rvalue rvalue_,
    //         model::ColumnType column_type_
    //         )
    //     : rvalue(rvalue_)
    //     {
    //         crow::json::type crow_json_type = rvalue.t();
    //     }
    //
    //     bool is_number(){return true;}
    //     bool is_text(){return true;}
    //     int64_t get_number(){return number;}
    //     std::string get_text(){return text;}
    // };
    entity_fields RepositoryImplSqlite::request_to_entity_fields(
        crow::json::rvalue& body, hive::essential::Crudl crudl)
    {
        entity_fields result;
        bool create = crudl == hive::essential::Crudl::Create;
        bool update = crudl == hive::essential::Crudl::Update;
        if (!create && !update)
        {
            throw std::runtime_error(std::string("Invalid crudl ") + crudl_to_string(crudl));
        }
        //id
        result.emplace_back(update ? static_cast<int64_t>(body["id"]) : 0);
        //created at
        if (create) { result.emplace_back(static_cast<int64_t>(util::Utils::current_unix_timestamp_ms())); }
        else { result.emplace_back(static_cast<int64_t>(0)); }
        //updated at
        result.emplace_back(static_cast<int64_t>(util::Utils::current_unix_timestamp_ms()));

        for (auto& col : model_definition.get_columns())
        {
            auto column_name = col.get_column_name();
            if (
                column_name == model::BaseColumns::ID ||
                column_name == model::BaseColumns::CREATED_AT ||
                column_name == model::BaseColumns::UPDATED_AT
            )
                continue;
            bool mandatory = col.is_mandatory();
            bool has_value = body.has(col.get_column_name());
            auto rvalue = has_value ? body[col.get_column_name()] : crow::json::rvalue();
            crow::json::type crow_json_type = has_value ? rvalue.t() : crow::json::type::Null;
            if (crow_json_type == crow::json::type::Null)
            {
                has_value = false;
            }
            bool auto_value = col.is_auto();
            bool internal_value = col.is_internal();

            auto column_type = col.get_column_type();
            auto primitive_column_type = column_type_to_primitive_column_type(column_type);

            if (auto_value ? has_value : false) debug << "rvalue=" << rvalue << commit;
            if (mandatory && !has_value)
            {
                throw std::runtime_error("Mandatory column " + col.get_column_name() + " is missing");
            }

            if (auto_value & !has_value)
            {
                switch (primitive_column_type)
                {
                case model::PrimitiveColumnType::Text:
                    {
                        result.emplace_back("");
                    }
                    break;
                case model::PrimitiveColumnType::Number:
                    {
                        result.emplace_back(cast64(0));
                    }
                    break;
                default: throw std::runtime_error("Unsupported type " + column_type_to_string(col.get_column_type()));
                }

                continue;
            }
            switch (primitive_column_type)
            {
            case model::PrimitiveColumnType::Text:
                {
                    result.emplace_back(
                        has_value
                            ? rvalue.s()
                            : col.get_default_value());
                }
                break;
            case model::PrimitiveColumnType::Number:
                {
                    if (has_value && crow_json_type == crow::json::type::String && rvalue == "")
                    {
                        result.emplace_back(cast64(0));
                    }
                    else
                    {
                        result.emplace_back(
                            has_value
                                ? cast64(rvalue)
                                : (col.get_default_value().empty()
                                       ? cast64(0)
                                       : cast64(col.get_default_i64_value())));
                    }
                    break;
                default: throw std::runtime_error("Unsupported type " + column_type_to_string(col.get_column_type()));
                }
            }
        }
        return result;
    }
}