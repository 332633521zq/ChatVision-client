#pragma once

#include "msgreceiver.h"
#include "msgsender.h"
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
    std::shared_ptr<MsgSender> _send_msg;
    std::shared_ptr<MsgReceiver> _recv_msg;
};
