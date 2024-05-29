cmake_minimum_required(VERSION 3.22)
project(PacketStickServer)

set(CMAKE_CXX_STANDARD 11)

find_package(Boost REQUIRED COMPONENTS system)

find_package(Protobuf REQUIRED)

add_executable(ProtobufClient ProtobufClient.cpp
        msg.pb.cc
        msg.pb.h)

target_link_libraries(ProtobufClient PRIVATE ${Boost_LIBRARIES} ${Protobuf_LIBRARIES})
