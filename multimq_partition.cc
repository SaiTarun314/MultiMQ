#include <iostream>
#include <grpcpp/grpcpp.h>
#include "partition_service.h"

int main(int argc, char** argv) {
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <port>" << std::endl;
        return 1;
    }

    std::string partitionName = argv[1];
    std::string port = argv[2];
    std::string server_address = "0.0.0.0:" + port;

    std::cout << "Partition Name: " << partitionName << ", Port: " << port << "\n";
    std::cout << "Server Address: " << server_address << "\n";

    Partition partition(partitionName);
    std::cout << "Partition created\n";

    PartitionServiceImpl service(partition);
    std::cout << "Service initialized\n";
    
    grpc::ServerBuilder builder;

    builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
    builder.RegisterService(&service);

    std::unique_ptr<grpc::Server> server(builder.BuildAndStart());
    
    server->Wait();

    return 0;
}
