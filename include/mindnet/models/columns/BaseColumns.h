#ifndef BASECOLUMNS_H
#define BASECOLUMNS_H

namespace mindnet::models::columns
{
    struct BaseColumns
    {
        BaseColumns() = delete;

        BaseColumns(const BaseColumns&) = delete;
        BaseColumns& operator=(const BaseColumns&) = delete;

        static constexpr const char* ID = "id";
        static constexpr const char* CREATED_AT = "created_at";
        static constexpr const char* UPDATED_AT = "updated_at";
    };
}

#endif // BASECOLUMNS_H