#ifndef CHANNEL_SERVICE_H
#define CHANNEL_SERVICE_H

#include <grpcpp/grpcpp.h>
#include "proto/channel.grpc.pb.h"
#include "channel.h"

class ChannelServiceImpl : public ChannelService::Service {
 public:
    ::grpc::Status Produce(::grpc::ServerContext* context, const ::ProduceRequest* request, ::ProduceResponse* response) override;
    ::grpc::Status Consume(::grpc::ServerContext* context, const ::ConsumeRequest* request, ::ConsumeResponse* response) override;
    ::grpc::Status HealthCheck(::grpc::ServerContext* context, const ::HealthCheckRequest* request, ::HealthCheckResponse* response) override;
};

#endif  