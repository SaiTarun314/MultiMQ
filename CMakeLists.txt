cmake_minimum_required(VERSION 3.13)
project(multimq LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 17)

find_package(Protobuf CONFIG REQUIRED)
find_package(gRPC CONFIG REQUIRED)

add_library(proto_def proto/hello.proto)

target_link_libraries(proto_def 
    gRPC::grpc++ 
)

target_include_directories(proto_def PUBLIC ${CMAKE_CURRENT_BINARY_DIR})
protobuf_generate(TARGET proto_def LANGUAGE cpp)
get_target_property(grpc_cpp_plugin_location gRPC::grpc_cpp_plugin LOCATION)
protobuf_generate(TARGET proto_def LANGUAGE grpc 
        GENERATE_EXTENSIONS .grpc.pb.h .grpc.pb.cc 
        PLUGIN "protoc-gen-grpc=${grpc_cpp_plugin_location}")

add_executable(multimq main.cpp)
target_link_libraries(multimq proto_def)