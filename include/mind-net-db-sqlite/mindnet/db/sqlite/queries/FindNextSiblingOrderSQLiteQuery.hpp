//
// Created by robertvokac on 9/7/25.
//
#ifndef FINDNEXTSIBLINGORDERSQLITEQUERY_H
#define FINDNEXTSIBLINGORDERSQLITEQUERY_H

#include "mindnet/api/Query.hpp"

namespace mindnet::db::sqlite::queries
{
    const std::string QUERY_FindNextSiblingOrder = "FindNextSiblingOrder";

    class FindNextSiblingOrderSQLiteQuery : public api::Query
    {
    public:
        FindNextSiblingOrderSQLiteQuery();

        ~FindNextSiblingOrderSQLiteQuery() = default;

        nlohmann::json call(nlohmann::json request) override;

    private:
    };
}
#endif // FINDNEXTSIBLINGORDERSQLITEQUERY_H
