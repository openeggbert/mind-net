//
// Created by robertvokac on 9/7/25.
//
#pragma once

#include "mindnet/api/Query.hpp"

namespace mindnet::db::sqlite::queries
{
    const std::string QUERY_Vacuum = "Vacuum";

    class VacuumSQLiteQuery : public api::Query
    {
    public:
        VacuumSQLiteQuery();

        ~VacuumSQLiteQuery() = default;

        nlohmann::json call(nlohmann::json request, api::InvalidateMethod& invalidate_method) override;

    private:
    };
}
