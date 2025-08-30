//
// Created by robertvokac on 8/16/25.
//
#ifndef ORDER_H
#define ORDER_H
#include "mindnet/Helper.h"


namespace mindnet::http
{
    enum class Order
    {
        ASC,
        DESC,
    };

    inline string order_to_string(const Order& order)
    {
        switch (order)
        {
        case Order::ASC: return "ASC";
        case Order::DESC: return "DESC";
        default: throw std::runtime_error(std::string("Unknown order: ") + std::to_string(cast64(order)));
        }
    }

    inline Order string_to_order(const string& string)
    {
        if (string == "ASC")
        {
            return Order::ASC;
        }
        if (string == "DESC")
        {
            return Order::DESC;
        }
        throw std::runtime_error(std::string("Unknown order: ") + string);
    }
}


#endif // ORDER_H
