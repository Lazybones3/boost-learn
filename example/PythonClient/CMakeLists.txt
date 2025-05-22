cmake_minimum_required(VERSION 3.22)
project(PythonClient)

set(CMAKE_CXX_STANDARD 11)

add_definitions(-D_WIN32_WINNT=0x0A00)

find_package(Boost REQUIRED COMPONENTS system)

# pybind11
#set(Python_ROOT_DIR "path to python document directory")
find_package(Python COMPONENTS Interpreter Development REQUIRED)
find_package(pybind11 CONFIG REQUIRED)
message(STATUS "Using pybind11 ${pybind11_VERSION}")

set(PROJECT_SRC PyTcpModule.cpp ../MyTcpClient/MyClient.cpp ../MyTcpClient/MyClient.h)

pybind11_add_module(PyTcpModule ${PROJECT_SRC})

target_link_libraries(PyTcpModule PRIVATE ${Boost_LIBRARIES} pybind11::module)
