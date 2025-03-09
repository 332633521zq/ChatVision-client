#pragma once

#include <boost/asio.hpp>
using boost::asio::ip::tcp;

class Client
{
public:
    Client(tcp::socket& sock, unsigned int uid);

private:
    unsigned int _uid;
    tcp::socket& _sock;
};
