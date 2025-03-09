#pragma once

// #include "sendnode.h"
#include "msgnode.h"
#include <boost/asio.hpp>
#include <queue>
#include <thread>

using boost::asio::ip::tcp;

class SendMsg
{
public:
    // SendMsg();
    SendMsg(boost::asio::ip::tcp::socket& sock);
    void SendRequest(char* msg, size_t msg_len, unsigned int object_id, short msgid);
    void SendRequest(std::string msg, unsigned int object_id, short msgid);

private:
    tcp::socket& _sock;
    std::mutex _mutex;
    std::condition_variable _consume;
    std::thread _send_thread;
    bool _b_stop = false;
    std::queue<std::shared_ptr<SendNode>> _send_que;
};
