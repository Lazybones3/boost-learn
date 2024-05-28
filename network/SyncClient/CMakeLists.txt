cmake_minimum_required(VERSION 3.22)
project(SyncClient)

set(CMAKE_CXX_STANDARD 11)

find_package(Boost REQUIRED COMPONENTS system)

add_executable(SyncClient SyncClient.cpp)

target_link_libraries(SyncClient PRIVATE ${Boost_LIBRARIES})
