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

#include <string>

namespace mindnet::model
{
    struct ModelDefinition;
}

namespace mindnet::orm
{
    struct QueryParams;
    using std::string;

    enum SelectMode
    {
        STAR, COUNT, IDS, IN_IDS
    };

    class SqlUtils
    {
    public:
        static string generate_select_one_sql(const std::string& table_name, const model::ModelDefinition& definition);
        static string generate_select_all_sql(
            const std::string& table_name,
            const orm::QueryParams& query_params,
            model::ModelDefinition& def,
            SelectMode select_mode = STAR,
            int id_count = 0);
        static string generate_select_count_sql(const std::string& table_name, const orm::QueryParams& query_params,
                                                model::ModelDefinition& def);
        static string generate_select_ids_sql(const std::string& table_name, const orm::QueryParams& query_params,
                                              model::ModelDefinition& def);
        string generate_select_in_ids_sql(const std::string& table_name, const orm::QueryParams& query_params,
                                          model::ModelDefinition& def, int id_count);
        static string generate_select_in_ids_sql(const std::string& table_name, const orm::QueryParams& query_params,
                                                 model::ModelDefinition& def);

        static string generate_insert_sql(const model::ModelDefinition& definition);
        static string generate_update_sql(const model::ModelDefinition& definition);
        static string generate_delete_sql(const model::ModelDefinition& definition);
    };
}