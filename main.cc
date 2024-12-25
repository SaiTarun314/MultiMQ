#include <iostream>
#include <grpcpp/grpcpp.h>
#include "proto/hello.grpc.pb.h"
#include "proto/hello.pb.h"
#include "proto/broker.grpc.pb.h"
#include "proto/broker.pb.h"
#include "broker.h"

class ProcessingServicesImpl : public ProcessingServices::Service {
    ::grpc::Status computeSum(::grpc::ServerContext* context, const ::Point3* request, ::Numeric* response) {
        std::cout << "In Compute Sum function" << std::endl;
        response->set_val(request->x() + request->y() + request->z());
        
        return grpc::Status::OK;
    }
};

class BrokerServiceImpl : public BrokerService::Service{
    private:
        Broker broker;
    public:
    ::grpc::Status Produce(::grpc::ServerContext* context, const ::ProduceRequest* request, ::ProduceResponse* response) {
        if(request->channel_id().size() == 0 || request->message().size() ==0) {
            response->set_success(false);
            return grpc::Status::OK;
        } else {
            broker.produce(request->channel_id(), request->message());
            response->set_success(true);
        }

        return grpc::Status::OK;
    }

    ::grpc::Status Consume(::grpc::ServerContext* context, const ::ConsumeRequest* request, ::ConsumeResponse* response) {
        std::string message = broker.consume(request->channel_id());

        if(request->channel_id().size() == 0 || message == "") {
            return grpc::Status::CANCELLED;
        }

        response->set_message(message);

        return grpc::Status::OK;
    }
};

int main() {
    std::cout << "Hello World!!!" << std::endl;

    // ProcessingServicesImpl service;
    BrokerServiceImpl service;
    grpc::ServerBuilder builder;

    builder.AddListeningPort("0.0.0.0:9999", grpc::InsecureServerCredentials());
    builder.RegisterService(&service);

    std::unique_ptr<grpc::Server> server(builder.BuildAndStart());
    
    server->Wait();

    return 0;
}
