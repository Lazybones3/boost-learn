#include <pybind11/pybind11.h>
#include "../MyTcpClient/MyClient.h"

namespace py = pybind11;

PYBIND11_MODULE(PyTcpModule, m) {
    py::class_<MyClient>(m, "MyClient")
            .def(py::init<std::string, int>())
            .def("connect", &MyClient::connect)
            .def("readMessage", &MyClient::readMessage)
            .def("writeMessage", &MyClient::writeMessage);
}
