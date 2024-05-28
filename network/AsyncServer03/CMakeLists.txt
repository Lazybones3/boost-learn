cmake_minimum_required(VERSION 3.22)
project(AsyncServer03)

set(CMAKE_CXX_STANDARD 11)

find_package(Boost REQUIRED COMPONENTS system)

add_executable(AsyncServer03 AsyncServer03.cpp
        CServer.cpp
        CServer.h
        CSession.cpp
        CSession.h)

target_link_libraries(AsyncServer03 PRIVATE ${Boost_LIBRARIES})
