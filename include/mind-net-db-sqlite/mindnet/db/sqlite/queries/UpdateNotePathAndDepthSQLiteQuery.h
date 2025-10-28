//
// Created by robertvokac on 9/7/25.
//
#ifndef UPDATENOTEPATHANDDEPTHSQLITEQUERY_H
#define UPDATENOTEPATHANDDEPTHSQLITEQUERY_H

#include "mindnet/api/Query.h"

namespace mindnet::db::sqlite::queries
{
    const std::string QUERY_UpdateNotePathAndDepth = "UpdateNotePathAndDepth";

    class UpdateNotePathAndDepthSQLiteQuery : public api::Query
    {
    public:
        UpdateNotePathAndDepthSQLiteQuery();

        ~UpdateNotePathAndDepthSQLiteQuery() = default;

        nlohmann::json call(nlohmann::json request) override;

    private:
    };
}
#endif // UPDATENOTEPATHANDDEPTHSQLITEQUERY_H
