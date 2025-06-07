//
// Created by admin on 2025/6/6.
//
#include "engine.h"

template<class T>
void Engine::match(Order &order, T &opposite_orders) {
    while (order.quantity > Decimal::ZERO && !opposite_orders.empty()) {
        auto best_price_level = opposite_orders.begin();
        Price best_price = best_price_level->first;

        // Check limit order price condition
        if (order.type == OrderType::ORDER_TYPE_LIMIT) {
            if ((order.side == OrderSide::ORDER_SIDE_BUY && order.price < best_price) ||
                (order.side == OrderSide::ORDER_SIDE_SELL && order.price > best_price)) {
                break; // No matching price
            }
        }

        OrderQueue &queue = best_price_level->second;
        while (!queue.empty() && order.quantity > Decimal::ZERO) {
            Order &maker = queue.front(); // FIFO
            Quantity trade_qty = std::min(order.quantity, maker.quantity);

            recordTrade(maker, order, trade_qty, maker.price);

            maker.quantity = maker.quantity - trade_qty;
            order.quantity = order.quantity - trade_qty;

            if (maker.quantity == Decimal::ZERO) {
                order_lookup.erase(maker.id);
                queue.pop_front();
            }
        }

        if (queue.empty()) {
            opposite_orders.erase(best_price_level);
        }
    }

    if (order.quantity > Decimal::ZERO && order.type == OrderType::ORDER_TYPE_LIMIT) {
        addToBook(order);
    }
}

void Engine::addToBook(const Order &order) {
    if (order.side == OrderSide::ORDER_SIDE_BUY) {
        BuyOrders &orders = buy_orders;
        orders[order.price].push_back(order);
        order_lookup[order.id] = &orders[order.price].back();
    } else if (order.side == OrderSide::ORDER_SIDE_SELL) {
        SellOrders &orders = sell_orders;
        orders[order.price].push_back(order);
        order_lookup[order.id] = &orders[order.price].back();
    }
}

void Engine::submitOrder(const Order &order) {
    Order local = order;
    if (order.side == OrderSide::ORDER_SIDE_BUY) {
        SellOrders &opposite_orders = sell_orders;
        match(local, opposite_orders);
    } else if (order.side == OrderSide::ORDER_SIDE_SELL) {
        BuyOrders &opposite_orders = buy_orders;
        match(local, opposite_orders);
    }
}

const std::vector<Trade> &Engine::getTradeList() const {
    return trade_list;
}

void Engine::recordTrade(const Order &maker, const Order &taker, const Quantity quantity, const Price price) {
    trade_list.push_back(
            {
                    .buy_order_id = (taker.side == OrderSide::ORDER_SIDE_BUY) ? taker.id : maker.id,
                    .sell_order_id = (taker.side == OrderSide::ORDER_SIDE_SELL) ? taker.id : maker.id,
                    .price = price,
                    .quantity = quantity,
                    .timestamp = std::max(taker.timestamp, maker.timestamp)
            }
    );
}

void Engine::cleanTradeList() {
    trade_list.clear();
}



