#pragma once

#include "recvmsg.h"
#include "sendmsg.h"
#include "user.h"
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

class Client
{
public:
    Client(tcp::socket& sock, unsigned int uid);

private:
    tcp::socket& _sock;
    std::shared_ptr<SendMsg> _send_msg;
    std::shared_ptr<RecvMsg> _recv_msg;
};
