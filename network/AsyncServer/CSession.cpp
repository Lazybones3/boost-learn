#include "CSession.h"
#include "CServer.h"

CSession::CSession(boost::asio::io_context &io_context, CServer *server)
    : _socket(io_context), _server(server) {
    boost::uuids::uuid a_uuid = boost::uuids::random_generator()();
    _uuid = boost::uuids::to_string(a_uuid);
}

tcp::socket &CSession::GetSocket() {
    return _socket;
}

std::string &CSession::GetUuid() {
    return _uuid;
}

void CSession::Start() {
    memset(_data, 0, MAX_LENGTH);
    _socket.async_read_some(boost::asio::buffer(_data, MAX_LENGTH), std::bind(&CSession::HandleRead, this, std::placeholders::_1, std::placeholders::_2, shared_from_this()));
}

void CSession::HandleRead(const boost::system::error_code error, std::size_t bytes_transferred, std::shared_ptr<CSession> _self_shared) {
    if (!error) {
        std::cout << "read data is " << _data << std::endl;
        Send(_data, bytes_transferred);
        memset(_data, 0, MAX_LENGTH);
        _socket.async_read_some(boost::asio::buffer(_data, MAX_LENGTH), std::bind(&CSession::HandleRead, this, std::placeholders::_1, std::placeholders::_2, _self_shared));
    } else {
        std::cerr << "handle read failed, error is " << error.message() << std::endl;
        _server->ClearSession(_uuid);
    }
}

void CSession::Send(char *msg, int max_length) {
    bool pending = false;
    std::lock_guard<std::mutex> lock(_send_lock);
    // 队列不为空，说明前面还有消息没发完
    if (!_send_que.empty()) {
        pending = true;
    }
    _send_que.push(std::make_shared<MsgNode>(msg, max_length));
    // 如果前面有消息没发完，则只入队不发送
    if (pending) {
        return;
    }
    boost::asio::async_write(_socket, boost::asio::buffer(msg, max_length), std::bind(&CSession::HandleWrite, this, std::placeholders::_1, shared_from_this()));
}

void CSession::HandleWrite(const boost::system::error_code error, std::shared_ptr<CSession> _self_shared) {
    if (!error) {
        std::lock_guard<std::mutex> lock(_send_lock);
        _send_que.pop();
        if (!_send_que.empty()) {
            auto &msg_node = _send_que.front();
            boost::asio::async_write(_socket, boost::asio::buffer(msg_node->_data, msg_node->_max_len), std::bind(&CSession::HandleWrite, this, std::placeholders::_1, _self_shared));
        }
    } else {
        std::cout << "handle write failed, error is " << error.message() << std::endl;
        _server->ClearSession(_uuid);
    }
}
