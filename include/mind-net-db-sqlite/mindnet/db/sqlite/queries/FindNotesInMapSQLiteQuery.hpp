//
// Created by robertvokac on 9/7/25.
//
#pragma once




#include "mindnet/api/Query.hpp"

namespace mindnet::db::sqlite::queries
{
    const std::string QUERY_FindNotesInMap = "FindNotesInMapQuery";

    class FindNotesInMapSQLiteQuery : public api::Query
    {
    public:
        FindNotesInMapSQLiteQuery();

        ~FindNotesInMapSQLiteQuery() = default;

        nlohmann::json call(nlohmann::json request) override;

    private:
    };
}
