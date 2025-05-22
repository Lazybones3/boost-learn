#include "MyClient.h"

#include <utility>

MyClient::MyClient(boost::asio::io_context& ioc, std::string host, short port) : _ioc(ioc), _host(std::move(host)), _port(port), _sock(_ioc) {}

void MyClient::connect() {
    tcp::endpoint remote_ep(make_address(_host), _port);
    boost::system::error_code error = boost::asio::error::host_not_found;
    _sock.connect(remote_ep, error);
    if (error) {
        std::cout << "connect failed, code is " << error.value() << " error msg is " << error.message();
    }
}

std::string MyClient::readMessage() {
    // message id
    char reply_id[HEAD_ID_LEN];
    size_t reply_length = boost::asio::read(_sock, boost::asio::buffer(reply_id, HEAD_ID_LEN));
    short msg_id = 0;
    memcpy(&msg_id, reply_id, HEAD_ID_LEN);
    // network byte order to host byte order
    short host_msg_id = boost::asio::detail::socket_ops::network_to_host_short(msg_id);
    std::cout << "Message id is " << host_msg_id << std::endl;
    // message head length
    char reply_head[HEAD_DATA_LEN];
    reply_length = boost::asio::read(_sock, boost::asio::buffer(reply_head, HEAD_DATA_LEN));
    short msg_len = 0;
    memcpy(&msg_len, reply_head, HEAD_DATA_LEN);
    short host_msg_len = boost::asio::detail::socket_ops::network_to_host_short(msg_len);
    std::cout << "Message len is " << host_msg_len << std::endl;
    char msg[MAX_LENGTH] = { 0 };
    size_t msg_length = boost::asio::read(_sock, boost::asio::buffer(msg, host_msg_len));
    return std::string{msg, static_cast<size_t>(host_msg_len)};
}

void MyClient::writeMessage(const std::string &msg) {
    const char* request = msg.data();
    short request_id = 1001;
    size_t request_length = strlen(request);
    char send_data[MAX_LENGTH] = { 0 };
    // host byte order to network byte order
    short net_request_id = boost::asio::detail::socket_ops::host_to_network_short(request_id);
    memcpy(send_data, &net_request_id, HEAD_ID_LEN);
    short net_reqeust_length = boost::asio::detail::socket_ops::host_to_network_short(request_length);
    memcpy(send_data + HEAD_ID_LEN, &net_reqeust_length, HEAD_DATA_LEN);
    memcpy(send_data + HEAD_TOTAL_LEN, request, request_length);
    boost::asio::write(_sock, boost::asio::buffer(send_data, request_length + HEAD_TOTAL_LEN));
}
