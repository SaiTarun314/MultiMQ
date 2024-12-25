#include <iostream>
#include <grpcpp/grpcpp.h>
#include "proto/hello.grpc.pb.h"
#include "proto/hello.pb.h"


class ProcessingServicesImpl : public ProcessingServices::Service {
    ::grpc::Status computeSum(::grpc::ServerContext* context, const ::Point3* request, ::Numeric* response) {
        std::cout<<"In Compute Sum function"<<std::endl;
        response->set_val(request->x() + request->y() + request->z());
        
        return grpc::Status::OK;
    }
};

int main() {
    std::cout<<"Hello World!!!"<<std::endl;

    ProcessingServicesImpl service;
    grpc::ServerBuilder builder;

    builder.AddListeningPort("0.0.0.0:9999", grpc::InsecureServerCredentials());
    builder.RegisterService(&service);

    std::unique_ptr<grpc::Server> server(builder.BuildAndStart());
    
    server->Wait();

    return 0;
}
