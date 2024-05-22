cmake_minimum_required(VERSION 3.22)
project(AsyncServer)

set(CMAKE_CXX_STANDARD 11)

find_package(Boost REQUIRED COMPONENTS system)

add_executable(AsyncServer main.cpp
        CServer.cpp
        CServer.h
        CSession.cpp
        CSession.h
        MsgNode.h)

target_link_libraries(AsyncServer PRIVATE ${Boost_LIBRARIES})
