#include "recvmsg.h"
#include <QDebug>
#include "ConstValue.h"
#include <iostream>
#include <nlohmann/json.hpp>

using namespace nlohmann;

RecvMsg::RecvMsg(boost::asio::ip::tcp::socket &sock, unsigned int uid)
    : _sock(sock)
    , _uid(uid)
{
    _recv_thread = std::thread(&RecvMsg::DealMsg, this);
    RegisterCallBacks();
    ReceiveMsg();
}

void RecvMsg::ReceiveMsg()
{
    for (;;) {
        char head[HEAD_TOTAL_LENGTH];
        boost::asio::read(_sock, boost::asio::buffer(head, HEAD_TOTAL_LENGTH));
        short msg_id;
        memcpy(&msg_id, head, HEAD_ID_LENGTH);
        msg_id = boost::asio::detail::socket_ops::network_to_host_short(msg_id);

        short msg_len;
        memcpy(&msg_len, head + 2, HEAD_DATA_LENGTH);
        msg_len = boost::asio::detail::socket_ops::network_to_host_short(msg_len);

        char msg_data[MAX_LENGTH];
        memset(msg_data, 0, MAX_LENGTH);

        boost::asio::read(_sock, boost::asio::buffer(msg_data, msg_len));

        std::shared_ptr<RecvNode> recv_node = std::make_shared<RecvNode>(msg_len, msg_id);
        memcpy(recv_node->_data, msg_data, msg_len);
        _recv_que.push(recv_node);

        _consume.notify_one();

        std::cout << "receive-----------------------------------------" << std::endl;
        std::cout << "msg_id:" << msg_id << std::endl;
        std::cout << "msg_len:" << msg_len << std::endl;
        std::cout << "head:" << head << std::endl;

        std::cout << "msg_data:" << msg_data << std::endl;
    }
}

void RecvMsg::DealMsg()
{
    for (;;) {
        std::cout << "DealMsg-----------------------------------\n";
        std::unique_lock<std::mutex> unique_lk(_mutex);

        // 队列为空，则用条件变量等待（挂起线程，加锁解锁需要消耗cpu资源）
        while (_recv_que.empty() && !_b_stop) {
            _consume.wait(unique_lk); // 线程挂起，释放资源，解锁，等待
        }

        // 客户端即将退出
        if (_b_stop) {
            while (!_recv_que.empty()) {
                auto msg_node = _recv_que.front();
                const short msg_id = msg_node->GetMsgId();
                auto call_back_iter = _fun_callbacks.find(msg_id);
                if (call_back_iter == _fun_callbacks.end()) {
                    _recv_que.pop();
                    continue;
                }
                call_back_iter->second(msg_id, std::string(msg_node->_data, msg_node->_total_len));
                _recv_que.pop();
            }
            return;
        }

        auto msg_node = _recv_que.front();
        const short msg_id = msg_node->GetMsgId();
        auto call_back_iter = _fun_callbacks.find(msg_id);
        if (call_back_iter == _fun_callbacks.end()) {
            _recv_que.pop();
            return;
        }
        call_back_iter->second(msg_id, std::string(msg_node->_data, msg_node->_total_len));
        _recv_que.pop();
    }
}

void RecvMsg::RegisterCallBacks()
{
    _fun_callbacks[MSG_HELLO_WORLD] = std::bind(&RecvMsg::HelloWorldCallBack,
                                                this,
                                                std::placeholders::_1,
                                                std::placeholders::_2);
}

void RecvMsg::HelloWorldCallBack(const short &msg_id, const std::string &msg_data)
{
    qDebug() << "HelloWorldCallBack----------------";
}
