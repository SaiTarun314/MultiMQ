#include "partition_service.h"

::grpc::Status PartitionServiceImpl::CreateChannel(::grpc::ServerContext* context, const ::PartitionCreateChannelRequest* request, ::PartitionCreateChannelResponse* response) {
    std::cout<<"Inside Create Channel\n";
    std::string channelId = request->channel_id();

    if(channelId.size() == 0) {
        response->set_success(false);
    }

    response->set_success(partition.createChannel(channelId));

    return grpc::Status::OK;
}

::grpc::Status PartitionServiceImpl::Produce(::grpc::ServerContext* context, const ::PartitionProduceRequest* request, ::PartitionProduceResponse* response)  {
    std::cout<<"Inside Produce\n";
    if (request->channel_id().empty() || request->message().empty()) {
        response->set_success(false);
        return grpc::Status(grpc::StatusCode::NOT_FOUND, "Requested Channel Id not Found");
    }

    bool isPublished = partition.produce(request->channel_id(), request->message());
    response->set_success(isPublished);
    return isPublished ? grpc::Status::OK : grpc::Status(grpc::StatusCode::INTERNAL, "Error Publishing Message to MQ");
}

::grpc::Status PartitionServiceImpl::Consume(::grpc::ServerContext* context, const ::PartitionConsumeRequest* request, ::PartitionConsumeResponse* response) {
    std::cout<<"Inside Consume\n";
    std::string message = partition.consume(request->channel_id());
    
    if (request->channel_id().empty() || message.empty()) {
        return grpc::Status(grpc::StatusCode::NOT_FOUND, "Requested Channel not Found");
    }

    response->set_message(message);
    return grpc::Status::OK;
}

::grpc::Status PartitionServiceImpl::UpdateOffset(::grpc::ServerContext* context, const ::PartitionOffsetRequest* request, ::PartitionOffsetResponse* response) {
    std::string channelId = request->channel_id();
    int offset = request->offset();

    response->set_success(partition.updateChannelOffset(channelId, offset));
    return grpc::Status::OK;
}

::grpc::Status PartitionServiceImpl::PartitionHealthCheck(::grpc::ServerContext* context, const ::PartitionHealthCheckRequest* request, ::PartitionHealthCheckResponse* response) {
    std::cout<<"Inside Partition HealthCheck\n";
    if(partition.partitionHealthCheck()) {
        response->set_healthy(true);
    } else {
        response->set_healthy(false);
    }

    return grpc::Status::OK;
}

::grpc::Status PartitionServiceImpl::ChannelHealthCheck(::grpc::ServerContext* context, const ::PartitionChannelHealthCheckRequest* request, ::PartitionHealthCheckResponse* response) {
    std::cout<<"Inside Partition Channel HealthCheck\n";
    if(partition.channelHealthCheck(request->channel_id())) {
        response->set_healthy(true);
    } else {
        response->set_healthy(false);
    }

    return grpc::Status::OK;
}