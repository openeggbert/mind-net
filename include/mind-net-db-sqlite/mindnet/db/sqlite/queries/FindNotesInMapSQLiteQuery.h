//
// Created by robertvokac on 9/7/25.
//
#ifndef FINDNOTESINMAPSQLITEQUERY_H
#define FINDNOTESINMAPSQLITEQUERY_H


#include "mindnet/api/Query.h"

namespace mindnet::db::sqlite::queries
{
    const std::string QUERY_FindNotesInMapQuery = "FindNotesInMapQuery";

    class FindNotesInMapSQLiteQuery : public api::Query
    {
    public:
        FindNotesInMapSQLiteQuery();

        ~FindNotesInMapSQLiteQuery() = default;

        nlohmann::json call(nlohmann::json request) override;

    private:

    };

}
#endif // FINDNOTESINMAPSQLITEQUERY_H
