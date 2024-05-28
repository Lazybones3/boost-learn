cmake_minimum_required(VERSION 3.22)
project(SyncClient04)

set(CMAKE_CXX_STANDARD 11)

find_package(Boost REQUIRED COMPONENTS system)

add_executable(SyncClient04 SyncClient04.cpp)

target_link_libraries(SyncClient04 PRIVATE ${Boost_LIBRARIES})
