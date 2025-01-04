#ifndef BROKER_SERVICE_H
#define BROKER_SERVICE_H

#include <grpcpp/grpcpp.h>
#include "proto/broker.grpc.pb.h"
#include "broker.h"

class BrokerServiceImpl : public BrokerService::Service {
 private:
    Broker broker;  
 public:
    ::grpc::Status Produce(::grpc::ServerContext* context, const ::BrokerProduceRequest* request, ::BrokerProduceResponse* response) override;
    ::grpc::Status Consume(::grpc::ServerContext* context, const ::BrokerConsumeRequest* request, ::BrokerConsumeResponse* response) override;
    ::grpc::Status CreateTopic(::grpc::ServerContext* context, const ::BrokerCreateTopicRequest* request, ::BrokerCreateTopicResponse* response) override;
    ::grpc::Status LinkPartition(::grpc::ServerContext* context, const ::BrokerLinkPartitionRequest* request, ::BrokerLinkPartitionResponse* response) override;
};

#endif  