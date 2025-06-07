//
// Created by admin on 2025/6/5.
//

#ifndef MATCH_TRADE_H
#define MATCH_TRADE_H

#include "type.h"

struct Trade {
    OrderID buy_order_id{};
    OrderID sell_order_id{};
    Price price;
    Quantity quantity;
    Timestamp timestamp{};
};
#endif //MATCH_TRADE_H
