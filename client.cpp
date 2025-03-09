#include "client.h"
#include "recvmsg.h"
#include "sendmsg.h"

using namespace std;
using namespace boost::asio::ip;

Client::Client(tcp::socket& sock, unsigned int uid)
    : _uid(uid)
    , _sock(sock)
{
    //创建上下文服务
    // boost::asio::io_context ioc;
    // //构造endpoint
    // tcp::endpoint remote_ep(make_address(IPADDRESS), PORT);
    // tcp::socket sock(ioc);
    // sock.connect(remote_ep);

    SendMsg(_sock, _uid);
    RecvMsg(_sock, _uid);
}
