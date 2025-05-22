#ifndef BOOSTLEARN_MYCLIENT_H
#define BOOSTLEARN_MYCLIENT_H

#include <boost/asio.hpp>
#include <iostream>

using namespace boost::asio::ip;
#define MAX_LENGTH (1024*2)
#define HEAD_TOTAL_LEN 4
#define HEAD_ID_LEN 2
#define HEAD_DATA_LEN 2

class MyClient {
public:
    MyClient(std::string host, short port);

    void connect();

    std::string readMessage();

    void writeMessage(std::string msg);

private:
    std::string _host;
    short _port;
    boost::asio::io_context _ioc;
    tcp::socket _sock;
};


#endif //BOOSTLEARN_MYCLIENT_H
