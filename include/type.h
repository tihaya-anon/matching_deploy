//
// Created by admin on 2025/6/5.
//

#ifndef MATCH_TYPE_H
#define MATCH_TYPE_H

#include <cstdint>
#include <string>
#include "decimal.h"

enum class OrderSide {
    ORDER_SIDE_BUY, ORDER_SIDE_SELL
};
enum class OrderType {
    ORDER_TYPE_LIMIT, ORDER_TYPE_MARKET
};

using OrderID = std::uint64_t;
using Price = Decimal;
using Quantity = Decimal;
using Timestamp = std::uint64_t;
#endif //MATCH_TYPE_H
