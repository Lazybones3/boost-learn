cmake_minimum_required(VERSION 3.22)
project(IOContextPoolServer)

set(CMAKE_CXX_STANDARD 11)

find_package(Boost REQUIRED COMPONENTS system)

find_package(jsoncpp REQUIRED)
get_target_property(JSON_INC_PATH jsoncpp_lib INTERFACE_INCLUDE_DIRECTORIES)
include_directories(${JSON_INC_PATH})

add_executable(IOContextPoolServer main.cpp
        CServer.cpp
        CServer.h
        CSession.cpp
        CSession.h
        MsgNode.cpp
        MsgNode.h
        LogicSystem.cpp
        LogicSystem.h
        Singleton.h
        AsioIOServicePool.cpp
        AsioIOServicePool.h
        const.h)

target_link_libraries(IOContextPoolServer PRIVATE ${Boost_LIBRARIES} jsoncpp_lib)
