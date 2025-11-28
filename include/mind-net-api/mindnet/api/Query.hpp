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

#pragma once

#include <memory>
#include <string>
#include <nlohmann/json.hpp>
#include "mindnet/essential/DatabaseType.hpp"
#include "mindnet/api/SqliteGlobal.hpp"
#include "mindnet/model/ModelDefinition.hpp"

namespace mindnet::api
{
    class InvalidateMethod
    {
    public:
        virtual ~InvalidateMethod() = default;
        virtual void invalidate(const model::ModelDefinition& def, identification id) = 0;
    };

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
            nlohmann::json& request, InvalidateMethod& invalidate_method
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

