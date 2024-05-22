#ifndef ASYNCSERVER_CSESSION_H
#define ASYNCSERVER_CSESSION_H
#define MAX_LENGTH 1024

#include <boost/asio.hpp>
#include <memory>
#include <queue>
#include <iostream>
#include <boost/uuid/uuid_io.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include "MsgNode.h"

using tcp = boost::asio::ip::tcp;

class CServer;

class CSession : public std::enable_shared_from_this<CSession> {
public:
    CSession(boost::asio::io_context &io_context, CServer *server);

    ~CSession() {
        std::cout << "Session destruct" << std::endl;
    }

    tcp::socket &GetSocket();

    std::string &GetUuid();

    void Start();

    void Send(char *msg, int max_length);

private:
    void HandleRead(const boost::system::error_code error, std::size_t bytes_transferred,
                    std::shared_ptr<CSession> _self_shared);

    void HandleWrite(const boost::system::error_code error, std::shared_ptr<CSession> _self_shared);

    tcp::socket _socket;
    std::string _uuid;
    char _data[MAX_LENGTH];
    CServer *_server;
    std::queue<std::shared_ptr<MsgNode> > _send_que;
    std::mutex _send_lock;
};


#endif //ASYNCSERVER_CSESSION_H
