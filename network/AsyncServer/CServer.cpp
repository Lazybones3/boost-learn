#include "CServer.h"

CServer::CServer(boost::asio::io_context &io_context, unsigned short port)
        : _io_context(io_context), _port(port), _acceptor(io_context, tcp::endpoint(tcp::v4(), port)) {
    StartAccept();
}

void CServer::StartAccept() {
    auto new_session = std::make_shared<CSession>(_io_context, this);
    _acceptor.async_accept(new_session->GetSocket(), std::bind(&CServer::HandleAccept, this, new_session, std::placeholders::_1));
}

void CServer::HandleAccept(std::shared_ptr<CSession> new_session, const boost::system::error_code &error) {
    if (!error) {
        new_session->Start();
        _sessions.insert(std::make_pair(new_session->GetUuid(), new_session));
    } else {
        std::cerr << "session accept failed, error is " << error.message() << std::endl;
    }
    StartAccept();
}

void CServer::ClearSession(const std::string& uuid) {
    _sessions.erase(uuid);
}
