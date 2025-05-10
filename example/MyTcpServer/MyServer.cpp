#include "MyServer.h"

MyServer::MyServer(boost::asio::io_context &ioc, short port) : _ioc(ioc), _port(port), _acceptor(ioc, tcp::endpoint(tcp::v4(), port)) {
    std::cout << "Server start success, listen on port : " << _port << std::endl;
    startAccept();
}

MyServer::~MyServer() {
    std::cout << "Server destruct listen on port : " << _port << std::endl;
}

void MyServer::startAccept() {
    std::shared_ptr<MySession> new_session = std::make_shared<MySession>(_ioc, this);
    _acceptor.async_accept(new_session->getSocket(), std::bind(&MyServer::handleAccept, this, new_session, std::placeholders::_1));
}

void MyServer::handleAccept(std::shared_ptr<MySession> new_session, const boost::system::error_code &error) {
    if (!error) {
        new_session->start();
        std::lock_guard<std::mutex> lock(_mutex);
        _sessions.insert(make_pair(new_session->getSessionId(), new_session));
    } else {
        std::cout << "Session accept failed, error is " << error.what() << std::endl;
    }
    startAccept();
}

void MyServer::clearSession(const std::string& id) {
    std::lock_guard<std::mutex> lock(_mutex);
    _sessions.erase(id);
}
