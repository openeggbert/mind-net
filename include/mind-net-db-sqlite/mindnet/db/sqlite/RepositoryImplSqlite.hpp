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
#pragma once


#include <vector>

#include "mindnet/api/IRepository.hpp"


namespace mindnet::db::sqlite
{
    using std::vector;

    class RepositoryImplSqlite : public api::IRepository
    {
    public:
        RepositoryImplSqlite(
            const model::ModelDefinition& model_definition
        );
        ~RepositoryImplSqlite() override;

        int create(const entity_fields& fields, string& error) override;
        entity_fields read(i64 id, string& error) override;
        bool update(i64 id, entity_fields& fields, string& error) override;
        bool remove(i64 id, string& error) override;
        std::vector<entity_fields> list(orm::QueryParams& query_params, string& error) override;
        std::vector<entity_fields> list_in_ids(std::vector<i64>& ids, string& error) override;
        std::vector<i64> list_ids(orm::QueryParams& query_params, string& error) override;
        [[nodiscard]] model::ModelDefinition& get_model_definition() override;
        entity_fields request_to_entity_fields(crow::json::rvalue& body, mindnet::essential::Crudl crudl) override;
    };
}
