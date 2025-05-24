#include "engine.hpp"
#include <functional>


template <typename PriceIDMap, typename IDOrderMap, typename Condition>
uint32_t process_order(Order& order, PriceIDMap& price_id_map, IDOrderMap& id_order_map, Condition cond) {
    uint32_t match_count = 0;
    
    for (auto price_it = price_id_map.begin(); price_it != price_id_map.end() && order.quantity > 0 && cond(price_it->first, order.price);) {
        auto& ids_at_price = price_it->second;
        for (auto id_it = ids_at_price.begin(); id_it != ids_at_price.end() && order.quantity > 0;) {
            Order& matched_order = id_order_map[*id_it];
            QuantityType traded_quantity = std::min(order.quantity, matched_order.quantity);
            order.quantity -= traded_quantity;
            matched_order.quantity -= traded_quantity;
            ++match_count;
            if (matched_order.quantity == 0) id_it = ids_at_price.erase(id_it); 
            else ++id_it;
        }
        if (ids_at_price.empty()) price_it = price_id_map.erase(price_it);
        else ++price_it;
    }

    return match_count;
}

uint32_t match_order(OrderBook& orderbook, const Order& incoming) {
    uint32_t match_count = 0;
    Order order = incoming;

    if (order.side == Side::BUY) {
        match_count = process_order(order, orderbook.sell_orders, orderbook.orders, std::less_equal<PriceType>());
        if (order.quantity > 0) orderbook.buy_orders[order.price].push_back(order.id);
    }
    else {
        match_count = process_order(order, orderbook.buy_orders, orderbook.orders, std::greater_equal<PriceType>());
        if (order.quantity > 0) orderbook.sell_orders[order.price].push_back(order.id);
    }

    return match_count;
}

void modify_order(OrderBook& orderbook, IDType order_id, QuantityType new_quantity) {
    orderbook.orders[order_id].quantity = new_quantity;
}
