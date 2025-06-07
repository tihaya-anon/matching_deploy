#include "engine.h"
#include "proto/engine.grpc.pb.h"
#include "proto/engine.pb.h"
#include <grpcpp/grpcpp.h>
#include <string>

class EngineServiceImpl : public EngineService::Service {
private:
  Engine engine;

  ::grpc::Status SubmitOrder(::grpc::ServerContext *context,
                             const ::SubmitOrderRequest *request,
                             ::SubmitOrderResponse *response) override {
    OrderSide order_side;
    OrderType order_type;

    switch (request->order().side()) {
    case OrderSideProto::ORDER_SIDE_BUY:
      order_side = OrderSide::ORDER_SIDE_BUY;
      break;
    case OrderSideProto::ORDER_SIDE_SELL:
      order_side = OrderSide::ORDER_SIDE_SELL;
      break;
    default:
      return grpc::Status(grpc::StatusCode::INVALID_ARGUMENT, "Invalid order side");
    }

    switch (request->order().type()) {
    case OrderTypeProto::ORDER_TYPE_LIMIT:
      order_type = OrderType::ORDER_TYPE_LIMIT;
      break;
    case OrderTypeProto::ORDER_TYPE_MARKET:
      order_type = OrderType::ORDER_TYPE_MARKET;
      break;
    default:
      return grpc::Status(grpc::StatusCode::INVALID_ARGUMENT, "Invalid order type");
    }

    engine.submitOrder({
      request->order().id(),
      order_side,
      order_type,
      Decimal::fromStr(request->order().price()),
      Decimal::fromStr(request->order().quantity()),
      request->order().timestamp()
    });

    return grpc::Status::OK;
  }

  ::grpc::Status GetTradeList(::grpc::ServerContext *context,
                              const ::GetTradeListRequest *request,
                              ::GetTradeListResponse *response) override {
    const auto &trades = engine.getTradeList();
    for (const auto &trade : trades) {
      TradeProto *proto_trade = response->add_tradelist();
      proto_trade->set_buy_order_id(trade.buy_order_id);
      proto_trade->set_sell_order_id(trade.sell_order_id);
      proto_trade->set_price(trade.price.toStr());
      proto_trade->set_quantity(trade.quantity.toStr());
      proto_trade->set_timestamp(trade.timestamp);
    }
    return grpc::Status::OK;
  }

  ::grpc::Status CleanTradeList(::grpc::ServerContext *context,
                                const ::CleanTradeListRequest *request,
                                ::GetTradeListResponse *response) override {
    engine.cleanTradeList();
    return grpc::Status::OK;
  }
};

int main(int argc, char **argv) {
  std::string server_address("0.0.0.0:50051");
  EngineServiceImpl service;

  grpc::ServerBuilder builder;
  builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
  builder.RegisterService(&service);

  std::unique_ptr<grpc::Server> server(builder.BuildAndStart());
  std::cout << "Server listening on " << server_address << std::endl;
  server->Wait();

  return 0;
}
