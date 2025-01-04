cmake_minimum_required(VERSION 3.13)
project(multimq_broker LANGUAGES CXX)
project(multimq_partition LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 17)

find_package(Protobuf CONFIG REQUIRED)
find_package(gRPC CONFIG REQUIRED)

add_library(proto_def proto/broker.proto proto/partition.proto)

target_link_libraries(proto_def 
    gRPC::grpc++ 
)

target_include_directories(proto_def PUBLIC ${CMAKE_CURRENT_BINARY_DIR})
protobuf_generate(TARGET proto_def LANGUAGE cpp)
get_target_property(grpc_cpp_plugin_location gRPC::grpc_cpp_plugin LOCATION)
protobuf_generate(TARGET proto_def LANGUAGE grpc 
        GENERATE_EXTENSIONS .grpc.pb.h .grpc.pb.cc 
        PLUGIN "protoc-gen-grpc=${grpc_cpp_plugin_location}")

# Add the broker library
add_library(broker
    lib/broker.cc include/broker.h
)

# Add the Channel library
add_library(channel
    lib/channel.cc include/channel.h
)

# Add the Partition library
add_library(partition
    lib/partition.cc include/partition.h
)

# Add the Broker Service Impl library
add_library(broker_service
    lib/broker_service.cc include/broker_service.h
)

# Add the Broker Service Impl library
add_library(partition_service
    lib/partition_service.cc include/partition_service.h
)

# Link dependencies for broker, channel, partition, partition_service and broker_service

target_include_directories(channel PUBLIC lib include)

target_include_directories(broker PUBLIC lib include)
target_link_libraries(broker partition proto_def)

target_include_directories(partition PUBLIC lib include)
target_link_libraries(partition channel)


target_include_directories(broker_service PUBLIC lib include proto_def)
target_link_libraries(broker_service broker proto_def)

target_include_directories(partition_service PUBLIC lib include proto_def)
target_link_libraries(partition_service channel partition proto_def)

add_executable(multimq_broker multimq_broker.cc)
add_executable(multimq_partition multimq_partition.cc)

target_link_libraries(multimq_broker broker_service broker proto_def)
target_link_libraries(multimq_partition partition_service channel partition proto_def)
target_include_directories(multimq_broker PUBLIC lib include)
target_include_directories(multimq_partition PUBLIC lib include)