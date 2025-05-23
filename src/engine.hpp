#pragma once

#include <map>
#include <vector>
#include <unordered_map>


enum class Side : unint8_t { BUY, SELL };

using IDType = uint32_t;
using PriceType = uint32_t;
using QuantityType = uint32_t; 


struct Order {
    IDType id;
    PriceType price;
    QuantityType quantity;
    Side side;
}


struct OrderBook {
    std::map<PriceType, std::vector<IDType>> sell_orders;
    std::map<PriceType, std::vector<IDType>, std::greater<IDType>> buy_orders;
    std::unordered_map<IDType, Order> orders;
};


uint32_t match_order(OrderBook& orderbook, const Order& incoming_order);

void modify_order(OrderBook& orderbook, IDType order_id, QuantityType new_quantity);

uint32_t get_volume_at_level(Orderbook& orderbook, Side side, PriceType quantity);

// Test functions
Order lookup_order_by_id(Orderbook& orderbook, IdType order_id);
bool order_exists(Orderbook& orderbook, IdType order_id);

Orderbook *create_orderbook();
