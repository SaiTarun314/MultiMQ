#ifndef BROKER_SERVICE_H
#define BROKER_SERVICE_H

#include <grpcpp/grpcpp.h>
#include "proto/broker.grpc.pb.h"
#include "broker.h"

class BrokerServiceImpl : public BrokerService::Service {
 private:
    Broker broker;  
 public:
    ::grpc::Status Produce(::grpc::ServerContext* context, const ::ProduceRequest* request, ::ProduceResponse* response) override;
    ::grpc::Status Consume(::grpc::ServerContext* context, const ::ConsumeRequest* request, ::ConsumeResponse* response) override;
    ::grpc::Status CreateTopic(::grpc::ServerContext* context, const ::CreateTopicRequest* request, ::CreateTopicResponse* response) override;
};

#endif  