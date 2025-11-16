//
// Created by robertvokac on 9/7/25.
//
#pragma once

#include <memory>
#include <string>
#include <nlohmann/json.hpp>
#include "mindnet/essential/DatabaseType.hpp"
#include "mindnet/api/SqliteGlobal.hpp"

namespace mindnet::api
{
    class Query
    {
    public:
        Query(
            const std::string query_name_,
            const std::string query_description_,
            const essential::DatabaseType database_type_)
            : name(query_name_),
              description(query_description_),
              database_type((database_type_))
        {
        }

        virtual ~Query() = default;

        virtual nlohmann::json call(
            nlohmann::json request

        ) = 0;
        //
        virtual const std::string& get_name() const { return name; }
        virtual const std::string& get_description() const { return description; }
        virtual const essential::DatabaseType& get_database_type() const { return database_type; }

    private:
        std::string name;
        std::string description;
        essential::DatabaseType database_type = essential::DatabaseType::Unknown;
    };

    typedef std::shared_ptr<Query> QueryPtr;
}

