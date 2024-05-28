cmake_minimum_required(VERSION 3.22)
project(SyncServer)

set(CMAKE_CXX_STANDARD 11)

find_package(Boost REQUIRED COMPONENTS system)

add_executable(SyncServer SyncServer.cpp)

target_link_libraries(SyncServer PRIVATE ${Boost_LIBRARIES})
