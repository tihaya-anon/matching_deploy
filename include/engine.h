//
// Created by admin on 2025/6/6.
//

#ifndef MATCH_ENGINE_H
#define MATCH_ENGINE_H

#include "deque"
#include "vector"
#include "map"
#include "unordered_map"
#include "decimal.h"
#include "order.h"
#include "trade.h"
#include "type.h"

using OrderQueue = std::deque<Order>;
using BuyOrders = std::map<Price, OrderQueue, std::greater<>>;
using SellOrders = std::map<Price, OrderQueue>;

class Engine {
private:
    BuyOrders buy_orders;
    SellOrders sell_orders;
    std::unordered_map<OrderID, Order *> order_lookup;
    std::vector<Trade> trade_list;


    template<class T>
    void match(Order &order, T &opposite_orders);

    void addToBook(const Order &order);

    void recordTrade(const Order &maker, const Order &taker, Quantity quantity, Price price);

public:
    void submitOrder(const Order &order);

    const std::vector<Trade> &getTradeList() const;

    void cleanTradeList();
};

#endif //MATCH_ENGINE_H
