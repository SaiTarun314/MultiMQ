#include <iostream>
#include <grpcpp/grpcpp.h>
#include "broker_service.h"

int main() {
    std::cout << "Hello World!!!" << std::endl;

    BrokerServiceImpl service;
    grpc::ServerBuilder builder;

    builder.AddListeningPort("0.0.0.0:9999", grpc::InsecureServerCredentials());
    builder.RegisterService(&service);

    std::unique_ptr<grpc::Server> server(builder.BuildAndStart());
    
    server->Wait();

    return 0;
}
