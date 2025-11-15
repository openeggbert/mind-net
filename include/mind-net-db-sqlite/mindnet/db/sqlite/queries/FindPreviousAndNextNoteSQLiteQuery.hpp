//
// Created by robertvokac on 9/7/25.
//
#ifndef FINDPREVIOUSANDNEXTNOTESQLITEQUERY_H
#define FINDPREVIOUSANDNEXTNOTESQLITEQUERY_H


#include "mindnet/api/Query.hpp"

namespace mindnet::db::sqlite::queries
{
    const std::string QUERY_FindPreviousAndNextNote = "FindPreviousAndNextNote";

    class FindPreviousAndNextNoteSQLiteQuery : public api::Query
    {
    public:
        FindPreviousAndNextNoteSQLiteQuery();

        ~FindPreviousAndNextNoteSQLiteQuery() = default;

        nlohmann::json call(nlohmann::json request) override;

    private:
    };
}
#endif // FINDPREVIOUSANDNEXTNOTESQLITEQUERY_H
