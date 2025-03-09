#pragma once

// #include "recvnode.h"
#include "msgnode.h"
#include <boost/asio.hpp>
#include <queue>
#include <thread>

using boost::asio::ip::tcp;

typedef std::function<void(const short& msg_id, const std::string& msg_data)> FunCallBack;

class RecvMsg
{
public:
    RecvMsg(boost::asio::ip::tcp::socket& io_context, unsigned int uid);
    void ReceiveMsg();
    void DealMsg();
    void RegisterCallBacks();
    void HelloWorldCallBack(const short& msg_id, const std::string& msg_data);

private:
    unsigned int _uid;
    tcp::socket& _sock;
    std::mutex _mutex;
    std::condition_variable _consume;
    std::thread _recv_thread;
    bool _b_stop = false;
    std::queue<std::shared_ptr<RecvNode>> _recv_que;
    std::unordered_map<short, FunCallBack> _fun_callbacks;
    char head[HEAD_TOTAL_LENGTH];
    char msg_data[MAX_LENGTH];
};
