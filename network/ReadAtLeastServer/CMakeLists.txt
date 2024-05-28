cmake_minimum_required(VERSION 3.22)
project(ReadAtLeastServer)

set(CMAKE_CXX_STANDARD 11)

find_package(Boost REQUIRED COMPONENTS system)

add_executable(ReadAtLeastServer ReadAtLeastServer.cpp
        CServer.cpp
        CServer.h
        CSession.cpp
        CSession.h)

target_link_libraries(ReadAtLeastServer PRIVATE ${Boost_LIBRARIES})
