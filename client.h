#pragma once

#include "recvmsg.h"
#include "sendmsg.h"
#include "user.h"
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

class Client
{
public:
    Client();
    void Start();

private:
    tcp::socket* _sock = nullptr;
    std::thread _logic_thread;
    std::shared_ptr<SendMsg> _send_msg;
    std::shared_ptr<RecvMsg> _recv_msg;
};
