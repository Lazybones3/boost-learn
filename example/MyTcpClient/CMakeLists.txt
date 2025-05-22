cmake_minimum_required(VERSION 3.22)
project(MyTcpClient)

set(CMAKE_CXX_STANDARD 11)

add_definitions(-D_WIN32_WINNT=0x0A00)

find_package(Boost REQUIRED COMPONENTS system)

aux_source_directory(${CMAKE_CURRENT_LIST_DIR} PROJECT_SRC)

file(GLOB PROJECT_HEADER ${CMAKE_CURRENT_LIST_DIR}/*.h)

add_executable(${PROJECT_NAME} ${PROJECT_SRC} ${PROJECT_HEADER})

target_link_libraries(${PROJECT_NAME} PRIVATE ${Boost_LIBRARIES})
