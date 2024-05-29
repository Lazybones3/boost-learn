cmake_minimum_required(VERSION 3.22)
project(ProtobufServer)

set(CMAKE_CXX_STANDARD 11)

find_package(Boost REQUIRED COMPONENTS system)

find_package(Protobuf REQUIRED)

add_executable(ProtobufServer ProtobufServer.cpp
        CServer.cpp
        CServer.h
        CSession.cpp
        CSession.h
        msg.pb.cc
        msg.pb.h
        const.h)

target_link_libraries(ProtobufServer PRIVATE ${Boost_LIBRARIES} ${Protobuf_LIBRARIES})
