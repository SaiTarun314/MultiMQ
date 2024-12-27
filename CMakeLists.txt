cmake_minimum_required(VERSION 3.13)
project(multimq LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 17)

find_package(Protobuf CONFIG REQUIRED)
find_package(gRPC CONFIG REQUIRED)

add_library(proto_def proto/hello.proto proto/broker.proto proto/channel.proto)

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

# Add the Broker Service Impl library
add_library(broker_service
    lib/broker_service.cc include/broker_service.h
)

# Link dependencies for broker, channel and broker_service

target_include_directories(broker_service PUBLIC lib include proto_def)
target_link_libraries(broker_service channel broker proto_def)
target_include_directories(broker PUBLIC lib include)
target_include_directories(channel PUBLIC lib include)

add_executable(multimq main.cc)
target_link_libraries(multimq broker_service broker channel proto_def)
target_include_directories(multimq PUBLIC lib include)