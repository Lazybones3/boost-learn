#include "MySession.h"
#include "MyServer.h"
#include <boost/uuid/uuid_io.hpp>
#include <boost/uuid/uuid_generators.hpp>

MySession::MySession(boost::asio::io_context &ioc, MyServer *server) : _socket(ioc), _server(server), _b_head_parse(false) {
    boost::uuids::uuid rand_id = boost::uuids::random_generator()();
    _session_id = boost::uuids::to_string(rand_id);
    _recv_head = std::make_shared<MsgNode>(HEAD_TOTAL_LEN);
}

MySession::~MySession() {
    std::cout << "MySession destruct" << std::endl;
}

tcp::socket &MySession::getSocket() {
    return _socket;
}

const std::string &MySession::getSessionId() {
    return _session_id;
}

void MySession::start() {
    asyncReadHead();
}

void MySession::close() {
    _socket.close();
    _server->clearSession(_session_id);
}

void MySession::asyncReadHead() {
    auto self = shared_from_this();
    asyncReadFull(HEAD_TOTAL_LEN, [self, this](const boost::system::error_code& ec, size_t transferred){
        try {
            if (ec) {
                std::cout << "handle read failed, error is " << ec.what() << std::endl;
                close();
                return;
            }

            if (transferred < HEAD_TOTAL_LEN) {
                std::cout << "read length not match, read [" << transferred << "] , total [" << HEAD_TOTAL_LEN << "]" << std::endl;
                close();
                return;
            }
            _recv_head->clear();
            // _data from asyncReadFull copy to _recv_head
            memcpy(_recv_head->data, _data, transferred);
            // parse message id
            short msg_id = 0;
            memcpy(&msg_id, _recv_head->data, HEAD_ID_LEN);
            // network byte order to host byte order
            msg_id = boost::asio::detail::socket_ops::network_to_host_short(msg_id);
            std::cout << "msg_id is " << msg_id << std::endl;
            // if id is illegal
            if (msg_id > MAX_LENGTH) {
                std::cout << "invalid msg_id is " << msg_id << std::endl;
                _server->clearSession(_session_id);
                return;
            }
            // parse message length
            short msg_len = 0;
            memcpy(&msg_len, _recv_head->data + HEAD_ID_LEN, HEAD_DATA_LEN);
            // network byte order to host byte order
            msg_len = boost::asio::detail::socket_ops::network_to_host_short(msg_len);
            std::cout << "msg_len is " << msg_len << std::endl;
            // if length is illegal
            if (msg_len > MAX_LENGTH) {
                std::cout << "invalid data length is " << msg_len << std::endl;
                _server->clearSession(_session_id);
                return;
            }
            _recv_body = std::make_shared<RecvNode>(msg_len, msg_id);
            asyncReadBody(msg_len);
        } catch (std::exception& e) {
            std::cout << "Exception code is " << e.what() << std::endl;
        }
    });
}

void MySession::asyncReadBody(int total_len) {
    auto self = shared_from_this();
    asyncReadFull(total_len, [self, this, total_len](const boost::system::error_code& ec, size_t transferred){
        try {
            if (ec) {
                std::cout << "handle read failed, error is " << ec.what() << std::endl;
                close();
                return;
            }
            if (transferred < total_len) {
                std::cout << "read length not match, read [" << transferred << "] , total [" << total_len<<"]" << std::endl;
                close();
                return;
            }
            // copy _data from asyncReadFull to _recv_body
            memcpy(_recv_body->data, _data, transferred);
            _recv_body->cur_len += transferred;
            // total_len has set in asyncReadHead
            // _recv_body->data[_recv_body->total_len] = '\0';
            std::cout << "receive data is " << _recv_body->data << std::endl;
            // respond to test
            if (_recv_body->_msg_id == MSG_IDS::LOGIN_REQ) {
                sendMessage("Welcome:" + std::string(_recv_body->data), MSG_IDS::LOGIN_RSP);
            }
            // continue to read header
            asyncReadHead();
        } catch (std::exception& e) {
            std::cout << "Exception code is " << e.what() << std::endl;
        }
    });
}

void MySession::asyncReadFull(size_t max_len, const std::function<void(const boost::system::error_code &, size_t)>& handler) {
    // clear _data
    memset(_data, 0, MAX_LENGTH);
    asyncReadLength(0, max_len, handler);
}

void MySession::asyncReadLength(size_t read_len, size_t total_len,
                                const std::function<void(const boost::system::error_code &, size_t)>& handler) {
    auto self = shared_from_this();
    _socket.async_read_some(boost::asio::buffer(_data + read_len, total_len - read_len), [read_len, total_len, handler, self](const boost::system::error_code& ec, size_t transferred){
        if (ec) {
            handler(ec, read_len + transferred);
            return;
        }
        // read enough length
        if (read_len + transferred >= total_len) {
            handler(ec, read_len + transferred);
            return;
        }
        // continue to read if length is not enough
        self->asyncReadLength(read_len + transferred, total_len, handler);
    });
}

void MySession::sendMessage(const std::string& msg, short msg_id) {
    std::lock_guard<std::mutex> lock(_send_lock);
    int send_que_size = _send_que.size();
    if (send_que_size > MAX_SENDQUE) {
        std::cout << "session: " << _session_id << " send queue fulled, size is " << MAX_SENDQUE << std::endl;
        return;
    }
    _send_que.push(std::make_shared<SendNode>(msg.c_str(), msg.length(), msg_id));
    // last sendMessage haven't finished
    if (send_que_size > 0) {
        return;
    }
    auto &msg_node = _send_que.front();
    auto self = shared_from_this();
    boost::asio::async_write(_socket, boost::asio::buffer(msg_node->data, msg_node->total_len), std::bind(&MySession::handleWrite, this, std::placeholders::_1, self));
}

void MySession::handleWrite(const boost::system::error_code &ec, std::shared_ptr<MySession> shared_self) {
    try {
        if (ec) {
            std::cout << "handle write failed, error is " << ec.what() << std::endl;
            close();
        } else {
            std::lock_guard<std::mutex> lock(_send_lock);
            // the front message has sent in sendMessage method
            _send_que.pop();
            if (!_send_que.empty()) {
                auto &msg_node = _send_que.front();
                boost::asio::async_write(_socket, boost::asio::buffer(msg_node->data, msg_node->total_len), std::bind(&MySession::handleWrite, this, std::placeholders::_1, shared_self));
            }
        }
    } catch (std::exception& e) {
        std::cerr << "Exception code : " << e.what() << std::endl;
    }
}
