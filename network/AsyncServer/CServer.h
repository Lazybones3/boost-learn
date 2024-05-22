#ifndef ASYNCSERVER_CSERVER_H
#define ASYNCSERVER_CSERVER_H

#include "CSession.h"

class CServer {
public:
    CServer(boost::asio::io_context &io_context, unsigned short port);

    void ClearSession(const std::string &uuid);

private:
    void StartAccept();

    void HandleAccept(std::shared_ptr<CSession> new_session, const boost::system::error_code &error);

    boost::asio::io_context &_io_context;
    unsigned short _port;
    tcp::acceptor _acceptor;
    std::map<std::string, std::shared_ptr<CSession> > _sessions;
};


#endif //ASYNCSERVER_CSERVER_H
