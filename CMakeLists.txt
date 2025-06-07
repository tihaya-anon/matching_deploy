cmake_minimum_required(VERSION 3.20)
project(server)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
find_package(Protobuf CONFIG REQUIRED)
find_package(gRPC CONFIG REQUIRED)

add_library(proto_lib proto/engine.proto)
target_link_libraries(proto_lib gRPC::grpc++)
target_include_directories(proto_lib PUBLIC ${CMAKE_CURRENT_BINARY_DIR})

get_target_property(grpc_cpp_plugin_location gRPC::grpc_cpp_plugin LOCATION)

protobuf_generate(TARGET proto_lib LANGUAGE cpp)
protobuf_generate(
  TARGET proto_lib LANGUAGE grpc
  GENERATE_EXTENSIONS .grpc.pb.h .grpc.pb.cc
  PLUGIN "protoc-gen-grpc=${grpc_cpp_plugin_location}"
)

add_executable(server main.cpp)
target_link_libraries(server proto_lib match_lib)