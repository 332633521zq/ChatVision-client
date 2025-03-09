#include "sendmsg.h"
#include <QDebug>
#include "ConstValue.h"
#include <iostream>
#include <nlohmann/json.hpp>

using namespace nlohmann;

SendMsg::SendMsg(boost::asio::ip::tcp::socket& sock, unsigned int uid)
    : _sock(sock)
    , _uid(uid)
{
    SendRequest("hello world", 20000001, MSG_HELLO_WORLD);
}

void SendMsg::SendRequest(std::string msg, unsigned int object_id, short msgid)
{
    char send_data[MAX_LENGTH] = {0};
    memset(send_data, 0, MAX_LENGTH);
    int msgid_host = boost::asio::detail::socket_ops::host_to_network_short(msgid);
    memcpy(send_data, &msgid_host, 2);

    json send_str;
    send_str["uid"] = _uid;
    send_str["object_id"] = object_id;
    send_str["data"] = msg;
    std::string temp_send_str = send_str.dump();

    int request_host_length = boost::asio::detail::socket_ops::host_to_network_short(
        temp_send_str.size());
    memcpy(send_data + 2, &request_host_length, 2);
    memcpy(send_data + 4, temp_send_str.c_str(), temp_send_str.size());

    std::cout << send_str.dump() << std::endl;
    boost::asio::write(_sock, boost::asio::buffer(send_data, temp_send_str.size() + 4));
}

void SendMsg::SendRequest(char* msg, size_t msg_len, unsigned int object_id, short msgid)
{
    char send_data[MAX_LENGTH] = {0};
    int msgid_host = boost::asio::detail::socket_ops::host_to_network_short(msgid);
    memcpy(send_data, &msgid_host, 2);

    json send_str;
    send_str["uid"] = _uid;
    send_str["object_id"] = object_id;
    send_str["data"] = std::string(msg, msg_len);
    std::string temp_send_str = send_str.dump();

    int request_host_length = boost::asio::detail::socket_ops::host_to_network_short(
        temp_send_str.size());
    memcpy(send_data + 2, &request_host_length, 2);
    memcpy(send_data + 4, temp_send_str.c_str(), temp_send_str.size());

    boost::asio::write(_sock, boost::asio::buffer(send_data, temp_send_str.size() + 4));
}
