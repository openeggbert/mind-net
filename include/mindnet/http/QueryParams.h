#ifndef QUERYPARAMS_H
#define QUERYPARAMS_H
#include <string>
#include <map>
#include <optional>
#include <set>

#include "Order.h"

namespace mindnet::http
{
    struct QueryParams
    {
        // pagination
        size_t page_number = 1; // default = first page
        size_t page_size = 20; // default = 20 items per page
        size_t total_items = 0;

        // sorting
        std::optional<std::string> sort; // e.g. "created_at"
        std::optional<mindnet::http::Order> order; // "asc" or "desc"

        // filters: key = field, value = filter value
        string_map filters;
        std::set<std::string> fields;

        // helper: add filter
        void add_filter(const std::string& key, const std::string& value)
        {
            filters[key] = value;
        }
        void remove_filter(const std::string& key)
        {
            filters.erase(key);
        }

    };
}
#endif // QUERYPARAMS_H
