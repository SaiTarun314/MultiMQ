#include "broker_service.h"

::grpc::Status BrokerServiceImpl::Produce(::grpc::ServerContext* context, const ::ProduceRequest* request, ::ProduceResponse* response) {
    if (request->topic_name().empty() || request->message().empty()) {
        response->set_success(false);
        return grpc::Status(grpc::StatusCode::NOT_FOUND, "Requested Topic not Found");
    }

    bool isPublished = broker.produce(request->topic_name(), request->message());
    response->set_success(isPublished);
    return isPublished ? grpc::Status::OK : grpc::Status(grpc::StatusCode::INTERNAL, "Error Publishing Message to MQ");
}

::grpc::Status BrokerServiceImpl::Consume(::grpc::ServerContext* context, const ::ConsumeRequest* request, ::ConsumeResponse* response) {
    std::string message = broker.consume(request->topic_name());

    if (request->topic_name().empty() || message.empty()) {
        return grpc::Status(grpc::StatusCode::NOT_FOUND, "Requested Topic not Found");
    }

    response->set_message(message);
    return grpc::Status::OK;
}

::grpc::Status BrokerServiceImpl::CreateTopic(::grpc::ServerContext* context, const ::CreateTopicRequest* request, ::CreateTopicResponse* response) {
    std::string topic_name= request->topic_name();

    if(topic_name.size() == 0) {
        return grpc::Status(grpc::StatusCode::INVALID_ARGUMENT, "Invalid Topic Name");
    } else {
        bool isCreated = broker.createTopic(topic_name);

        auto ALREADY_EXISTS = grpc::Status(grpc::StatusCode::ALREADY_EXISTS, "The Topic " + topic_name + " already exists!");
        auto CREATED = grpc::Status::OK;
        response->set_success(isCreated);

        return isCreated ? CREATED : ALREADY_EXISTS;
    }
}