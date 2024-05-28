cmake_minimum_required(VERSION 3.22)
project(AsyncServer01)

set(CMAKE_CXX_STANDARD 11)

find_package(Boost REQUIRED COMPONENTS system)

add_executable(AsyncServer01 AsyncServer01.cpp
        Session.cpp
        Session.h)

target_link_libraries(AsyncServer01 PRIVATE ${Boost_LIBRARIES})
