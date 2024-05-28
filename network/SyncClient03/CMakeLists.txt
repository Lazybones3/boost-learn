cmake_minimum_required(VERSION 3.22)
project(SyncClient03)

set(CMAKE_CXX_STANDARD 11)

find_package(Boost REQUIRED COMPONENTS system)

add_executable(SyncClient03 SyncClient03.cpp)

target_link_libraries(SyncClient03 PRIVATE ${Boost_LIBRARIES})
