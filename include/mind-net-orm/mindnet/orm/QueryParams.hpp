/*
 * MIT License
 * Copyright (c) 2025 Robert Vokac
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */
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
        mutable size_t total_items = 0;

        // sorting
        std::optional<std::string> sort; // e.g. "created_at"
        std::optional<mindnet::orm::Order> order; // "asc" or "desc"

        // filters: key = field, value = filter value
        string_map filters;
        std::set<std::string> fields;
        std::vector<identification> ids;

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