#pragma once

#include <string>
#include <map>
#include <optional>
#include <set>

#include "Order.hpp"

namespace mindnet::orm
{
    struct QueryParams
    {
        // pagination
        size_t page_number = 1; // default = first page
        size_t page_size = 20; // default = 20 items per page
        size_t total_items = 0;

        // sorting
        std::optional<std::string> sort; // e.g. "created_at"
        std::optional<mindnet::orm::Order> order; // "asc" or "desc"

        // filters: key = field, value = filter value
        string_map filters;
        std::set<std::string> fields;
        std::vector<i64> ids;

        // helper: add filter
        void add_filter(const std::string& key, const std::string& value)
        {
            filters[key] = value;
        }

        // helper: add filter
        void add_filter(const std::string& key, const i64 value)
        {
            filters[key] = std::to_string(value);
        }

        void remove_filter(const std::string& key)
        {
            filters.erase(key);
        }

        [[nodiscard]] std::string to_json() const
        {
            std::string json = "{";

            // Add pagination
            json += "\"page_number\":" + std::to_string(page_number) + ",";
            json += "\"page_size\":" + std::to_string(page_size) + ",";
            json += "\"total_items\":" + std::to_string(total_items) + ",";

            // Add sorting
            json += "\"sort\":";
            if (sort.has_value())
            {
                json += "\"" + sort.value() + "\"";
            }
            else
            {
                json += "null";
            }
            json += ",";

            json += "\"order\":";
            if (order.has_value())
            {
                json += "\"" + std::string(order.value() == Order::Asc ? "asc" : "desc") + "\"";
            }
            else
            {
                json += "null";
            }
            json += ",";

            // Add filters
            json += "\"filters\":{";
            bool first = true;
            for (const auto& [key, value] : filters)
            {
                if (!first) json += ",";
                json += "\"" + key + "\":\"" + value + "\"";
                first = false;
            }
            json += "},";

            // Add fields
            json += "\"fields\":[";
            first = true;
            for (const auto& field : fields)
            {
                if (!first) json += ",";
                json += "\"" + field + "\"";
                first = false;
            }
            json += "]";

            json += "}";
            return json;
        }
    };
}
