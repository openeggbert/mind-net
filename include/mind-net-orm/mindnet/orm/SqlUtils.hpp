//
// Created by robertvokac on 9/23/25.
//
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
