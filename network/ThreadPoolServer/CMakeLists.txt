cmake_minimum_required(VERSION 3.22)
project(ThreadPoolServer)

set(CMAKE_CXX_STANDARD 11)

find_package(Boost REQUIRED COMPONENTS system)

find_package(jsoncpp REQUIRED)
get_target_property(JSON_INC_PATH jsoncpp_lib INTERFACE_INCLUDE_DIRECTORIES)
include_directories(${JSON_INC_PATH})

add_executable(ThreadPoolServer main.cpp
        CServer.cpp
        CServer.h
        CSession.cpp
        CSession.h
        MsgNode.cpp
        MsgNode.h
        LogicSystem.cpp
        LogicSystem.h
        Singleton.h
        AsioThreadPool.cpp
        AsioThreadPool.h
        const.h)

target_link_libraries(ThreadPoolServer PRIVATE ${Boost_LIBRARIES} jsoncpp_lib)
