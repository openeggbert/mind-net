//
// Created by robertvokac on 9/7/25.
//
#ifndef GETRSESSIONSELECTEDITEMSSQLITEQUERY_H
#define GETRSESSIONSELECTEDITEMSSQLITEQUERY_H


#include "mindnet/api/Query.h"

namespace mindnet::db::sqlite::queries
{
    const std::string QUERY_GetRSessionSelectedItemsQuery = "GetRSessionSelectedItems";

    class GetRSessionSelectedItemsSQLiteQuery : public api::Query
    {
    public:
        GetRSessionSelectedItemsSQLiteQuery();

        ~GetRSessionSelectedItemsSQLiteQuery() = default;

        nlohmann::json call(nlohmann::json request) override;

    private:
    };
}
#endif // GETRSESSIONSELECTEDITEMSSQLITEQUERY_H
