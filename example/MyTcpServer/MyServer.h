#ifndef BOOSTLEARN_MYSERVER_H
#define BOOSTLEARN_MYSERVER_H

#include <map>
#include "MySession.h"

class MyServer {
public:
    MyServer(boost::asio::io_context& ioc, short port);
    ~MyServer();
    void clearSession(const std::string& id);
private:
    void startAccept();
    void handleAccept(std::shared_ptr<MySession> new_session, const boost::system::error_code & error);
private:
    boost::asio::io_context &_ioc;
    short _port;
    tcp::acceptor _acceptor;
    std::map<std::string, std::shared_ptr<MySession>> _sessions;
    std::mutex _mutex;
};


#endif //BOOSTLEARN_MYSERVER_H
