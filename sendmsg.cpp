#include "sendmsg.h"
#include <QDebug>
#include "ConstValue.h"
#include "user.h"
#include <iostream>
#include <nlohmann/json.hpp>

using namespace nlohmann;

SendMsg::SendMsg()
    : _token_bucket(std::make_shared<TokenBucket>(3, 2))
{}

SendMsg::~SendMsg() {}

void SendMsg::SetSocket(boost::asio::ip::tcp::socket* sock)
{
    _sock = sock;
}

void SendMsg::SendRequest(std::string msg, unsigned int object_id, short msgid)
{
    // std::lock_guard<std::mutex> lock(_mutex);

    json send_str;
    send_str["uid"] = User::GetInstance()->GetUid();
    send_str["object_id"] = object_id;
    send_str["data"] = msg;
    std::string temp_send_str = send_str.dump();

    std::shared_ptr<SendNode> new_node = std::make_shared<SendNode>(temp_send_str.c_str(),
                                                                    temp_send_str.length(),
                                                                    object_id,
                                                                    msgid);

    _send_que.push(new_node);

    while (_send_que.size() >= 1) {
        bool is_success = _token_bucket->acquire(1);

        if (!is_success) {
            sleep(1);
            continue;
        }

        auto& send_node = _send_que.front();
        boost::asio::write(*_sock, boost::asio::buffer(send_node->_data, send_node->_total_len));
        _send_que.pop();
    }
}

void SendMsg::SendRequest(char* msg, size_t msg_len, unsigned int object_id, short msgid)
{
    json send_str;
    send_str["uid"] = User::GetInstance()->GetUid();
    send_str["object_id"] = object_id;
    send_str["data"] = std::string(msg, msg_len);
    std::string temp_send_str = send_str.dump();

    std::shared_ptr<SendNode> new_node = std::make_shared<SendNode>(temp_send_str.c_str(),
                                                                    temp_send_str.length(),
                                                                    object_id,
                                                                    msgid);

    _send_que.push(new_node);

    while (_send_que.size() >= 1) {
        bool is_success = _token_bucket->acquire(1);

        if (!is_success) {
            sleep(1);
            continue;
        }

        auto& send_node = _send_que.front();
        boost::asio::write(*_sock, boost::asio::buffer(send_node->_data, send_node->_total_len));
        _send_que.pop();
    }
}
