//
// Created by robertvokac on 9/7/25.
//
#ifndef GETSELECTEDITEMSSQLITEQUERY_H
#define GETSELECTEDITEMSSQLITEQUERY_H

#include "mindnet/api/Query.h"

namespace mindnet::db::sqlite::queries
{
    const std::string QUERY_GetSelectedItemsQuery = "GetSelectedItemsQuery";

    class GetSelectedItemsSQLiteQuery : public api::Query
    {
    public:
        GetSelectedItemsSQLiteQuery();

        ~GetSelectedItemsSQLiteQuery() = default;

        nlohmann::json call(nlohmann::json request) override;

    private:

    };

}
#endif // GETSELECTEDITEMSSQLITEQUERY_H
