//
// Created by admin on 2025/6/5.
//

#ifndef MATCH_ORDER_H
#define MATCH_ORDER_H

#include "type.h"

struct Order {
    OrderID id{};
    OrderSide side{};
    OrderType type{};
    Price price;
    Quantity quantity;
    Timestamp timestamp{};
};
#endif //MATCH_ORDER_H
