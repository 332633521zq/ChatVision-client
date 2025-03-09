#include "client.h"
#include "filetools.h"

using namespace std;
using namespace boost::asio::ip;

Client::Client(tcp::socket& sock, unsigned int uid)
    : _sock(sock)
{
    //创建上下文服务
    // boost::asio::io_context ioc;
    // //构造endpoint
    // tcp::endpoint remote_ep(make_address(IPADDRESS), PORT);
    // tcp::socket sock(ioc);
    // sock.connect(remote_ep);

    User::GetInstance()->SetUid(uid);

    _send_msg = std::make_shared<SendMsg>(_sock);
    _recv_msg = std::make_shared<RecvMsg>(_sock);
}
