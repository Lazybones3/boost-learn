#ifndef BOOSTLEARN_MYSESSION_H
#define BOOSTLEARN_MYSESSION_H

#include <iostream>
#include <memory>
#include <queue>
#include <boost/asio.hpp>
#include "MsgNode.h"

using boost::asio::ip::tcp;

#define MAX_SENDQUE 1000

class MyServer;

class MySession : public std::enable_shared_from_this<MySession> {
public:
    MySession(boost::asio::io_context& ioc, MyServer* server);
    ~MySession();
    tcp::socket& getSocket();
    const std::string& getSessionId();
    void start();
    void close();
    // method for receive message
    void asyncReadHead();
    void asyncReadBody(int total_len);
    // method for send message
    void sendMessage(const std::string& msg, short msg_id);
private:
    void asyncReadFull(size_t max_len, const std::function<void(const boost::system::error_code&, size_t)>& handler);
    void asyncReadLength(size_t read_len, size_t total_len, const std::function<void(const boost::system::error_code&, size_t)>& handler);
    void handleWrite(const boost::system::error_code& ec, std::shared_ptr<MySession> shared_self);
private:
    tcp::socket _socket;
    std::string _session_id;
    MyServer* _server;
    // properties for receive message
    char _data[MAX_LENGTH];
    bool _b_head_parse;
    std::shared_ptr<MsgNode> _recv_head;
    std::shared_ptr<RecvNode> _recv_body;
    // properties for send message
    std::queue<std::shared_ptr<SendNode>> _send_que;
    std::mutex _send_lock;
};


#endif //BOOSTLEARN_MYSESSION_H
