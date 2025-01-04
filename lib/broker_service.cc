#include "broker_service.h"

::grpc::Status BrokerServiceImpl::Produce(::grpc::ServerContext* context, const ::BrokerProduceRequest* request, ::BrokerProduceResponse* response) {
    if (request->topic_name().empty() || request->message().empty()) {
        response->set_success(false);
        return grpc::Status(grpc::StatusCode::NOT_FOUND, "Requested Topic not Found");
    }

    bool isPublished = broker.produce(request->topic_name(), request->message());
    response->set_success(isPublished);
    return isPublished ? grpc::Status::OK : grpc::Status(grpc::StatusCode::INTERNAL, "Error Publishing Message to MQ");
}

::grpc::Status BrokerServiceImpl::Consume(::grpc::ServerContext* context, const ::BrokerConsumeRequest* request, ::BrokerConsumeResponse* response) {
    std::string message = broker.consume(request->topic_name());

    if (request->topic_name().empty() || message.empty()) {
        return grpc::Status(grpc::StatusCode::NOT_FOUND, "Requested Topic not Found");
    }

    response->set_message(message);
    return grpc::Status::OK;
}

::grpc::Status BrokerServiceImpl::CreateTopic(::grpc::ServerContext* context, const ::BrokerCreateTopicRequest* request, ::BrokerCreateTopicResponse* response) {
    std::string topic_name= request->topic_name();
    int replication_factor = request->replication_factor();

    if(topic_name.size() == 0) {
        return grpc::Status(grpc::StatusCode::INVALID_ARGUMENT, "Invalid Topic Name");
    } else {
        bool isCreated = broker.createTopic(topic_name, replication_factor);

        auto ALREADY_EXISTS = grpc::Status(grpc::StatusCode::ALREADY_EXISTS, "The Topic " + topic_name + " already exists!");
        auto CREATED = grpc::Status::OK;
        response->set_success(isCreated);

        return isCreated ? CREATED : ALREADY_EXISTS;
    }
}

::grpc::Status BrokerServiceImpl::LinkPartition(::grpc::ServerContext* context, const ::BrokerLinkPartitionRequest* request, ::BrokerLinkPartitionResponse* response) {
    std::string partitionName = request->partition_name();
    int port = request->port();

    bool isSuccess = broker.linkPartitionToBroker(partitionName, port);

    response->set_success(isSuccess);
    return isSuccess ? grpc::Status::OK : grpc::Status(grpc::StatusCode::INTERNAL, "Error in Linking the Partition to the Broker");
}