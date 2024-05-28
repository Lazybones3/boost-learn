cmake_minimum_required(VERSION 3.22)
project(AsyncServer04)

set(CMAKE_CXX_STANDARD 11)

find_package(Boost REQUIRED COMPONENTS system)

add_executable(AsyncServer04 AsyncServer04.cpp
        CServer.cpp
        CServer.h
        CSession.cpp
        CSession.h
        const.h)

target_link_libraries(AsyncServer04 PRIVATE ${Boost_LIBRARIES})
