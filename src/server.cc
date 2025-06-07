#include "proto/engine.grpc.pb.h"
#include "proto/engine.pb.h"
#include <grpcpp/grpcpp.h>

class EngineServiceImpl : public EngineService::Service {
  ::grpc::Status SubmitOrder(::grpc::ServerContext *context,
                             const ::SubmitOrderRequest *request,
                             ::SubmitOrderResponse *response) {
    return grpc::Status::OK;
  };
  ::grpc::Status GetTradeList(::grpc::ServerContext *context,
                              const ::GetTradeListRequest *request,
                              ::GetTradeListResponse *response) {
    return grpc::Status::OK;
  };
  ::grpc::Status CleanTradeList(::grpc::ServerContext *context,
                                const ::CleanTradeListRequest *request,
                                ::GetTradeListResponse *response) {
    return grpc::Status::OK;
  };
};

int main() { return 0; }