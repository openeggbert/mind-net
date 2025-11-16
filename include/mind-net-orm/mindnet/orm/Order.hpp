//
// Created by robertvokac on 8/16/25.
//
#pragma once

#include <stdexcept>
#include "mindnet/essential/DatabaseType.hpp"
#include "mindnet/essential/Helper.hpp"

namespace mindnet::orm
{
    enum class Order
    {
        Asc,
        Desc,
    };

    inline string order_to_string(const Order& order)
    {
        switch (order)
        {
        case Order::Asc: return "ASC";
        case Order::Desc: return "DESC";
        default: throw std::runtime_error(std::string("Unknown order: ") + std::to_string(cast64(order)));
        }
    }

    inline Order string_to_order(const string& string)
    {
        if (string == "ASC" || string == "asc")
        {
            return Order::Asc;
        }
        if (string == "DESC" || string == "desc")
        {
            return Order::Desc;
        }
        throw std::runtime_error(std::string("Unknown order: ") + string);
    }
}
