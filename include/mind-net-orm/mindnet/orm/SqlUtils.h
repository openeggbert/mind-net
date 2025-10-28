//
// Created by robertvokac on 9/23/25.
//
#ifndef SQLUTILS_H
#define SQLUTILS_H

#include <string>

namespace mindnet::model
{
    struct ModelDefinition;
}

namespace mindnet::orm
{
    struct QueryParams;
    using std::string;

    class SqlUtils
    {
    public:
        static string generate_select_one_sql(const std::string& table_name);
        static string generate_select_all_sql(
            const std::string& table_name,
            const orm::QueryParams& query_params,
            model::ModelDefinition& def,
            bool count = false);
        static string generate_select_count_sql(const std::string& table_name, const orm::QueryParams& query_params,
                                                model::ModelDefinition& def);

        static string generate_insert_sql(const model::ModelDefinition& definition);
        static string generate_update_sql(const model::ModelDefinition& definition);
        static string generate_delete_sql(const model::ModelDefinition& definition);
    };
}
#endif // SQLUTILS_H
