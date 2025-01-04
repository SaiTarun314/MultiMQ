#ifndef PARTITION_SERVICE_H
#define PARTITION_SERVICE_H

#include <grpcpp/grpcpp.h>
#include "proto/partition.grpc.pb.h"
#include "partition.h"

class PartitionServiceImpl : public PartitionService::Service {
 private: 
      Partition partition;
 public:
      explicit PartitionServiceImpl(const Partition& partition) : partition(partition) {}
      ::grpc::Status CreateChannel(::grpc::ServerContext* context, const ::PartitionCreateChannelRequest* request, ::PartitionCreateChannelResponse* response) override;
      ::grpc::Status Produce(::grpc::ServerContext* context, const ::PartitionProduceRequest* request, ::PartitionProduceResponse* response) override;
      ::grpc::Status Consume(::grpc::ServerContext* context, const ::PartitionConsumeRequest* request, ::PartitionConsumeResponse* response) override;
      ::grpc::Status PartitionHealthCheck(::grpc::ServerContext* context, const ::PartitionHealthCheckRequest* request, ::PartitionHealthCheckResponse* response) override;
      ::grpc::Status HealthCheck(::grpc::ServerContext* context, const ::PartitionChannelHealthCheckRequest* request, ::PartitionHealthCheckResponse* response) override;
};

#endif  