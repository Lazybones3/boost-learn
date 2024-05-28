cmake_minimum_required(VERSION 3.22)
project(PacketStickClient)

set(CMAKE_CXX_STANDARD 11)

find_package(Boost REQUIRED COMPONENTS system)

add_executable(PacketStickClient TestClient.cpp)

target_link_libraries(PacketStickClient PRIVATE ${Boost_LIBRARIES})
