//
// Created by robertvokac on 9/7/25.
//
#pragma once

#include "mindnet/api/Query.hpp"

namespace mindnet::db::sqlite::queries
{
    const std::string QUERY_CleanupHistoryOrphans = "CleanupHistoryOrphans";

    class CleanupHistoryOrphansSQLiteQuery : public api::Query
    {
        static constexpr long long MILLISECONDS_PER_DAY = 24L * 60L * 60L * 1000L;
    public:
        CleanupHistoryOrphansSQLiteQuery();

        ~CleanupHistoryOrphansSQLiteQuery() = default;

        nlohmann::json call(nlohmann::json request) override;

    private:
    };
}
