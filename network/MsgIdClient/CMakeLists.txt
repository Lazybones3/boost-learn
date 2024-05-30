cmake_minimum_required(VERSION 3.22)
project(MsgIdClient)

set(CMAKE_CXX_STANDARD 11)

find_package(Boost REQUIRED COMPONENTS system)

find_package(jsoncpp REQUIRED)
get_target_property(JSON_INC_PATH jsoncpp_lib INTERFACE_INCLUDE_DIRECTORIES)
include_directories(${JSON_INC_PATH})

add_executable(MsgIdClient MsgIdClient.cpp)

target_link_libraries(MsgIdClient PRIVATE ${Boost_LIBRARIES} jsoncpp_lib)
