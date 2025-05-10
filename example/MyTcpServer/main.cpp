#include "MyServer.h"

int main() {
    try {
        boost::asio::io_context  io_context;
        MyServer s(io_context, 10086);
        io_context.run();
    } catch(std::exception & e){
        std::cerr << "Exception: " << e.what() <<  std::endl;
    }
    boost::asio::io_context io_context;
    return 0;
}