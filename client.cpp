#include "client.h"
#include "filetools.h"

using namespace std;
using namespace boost::asio::ip;

#define IPADDRESS "127.0.0.1"
// #define IPADDRESS "10.252.78.90"
// #define IPADDRESS "10.253.5.10"

// #define IPADDRESS "192.168.83.186"

#define PORT 10086

Client &Client::getInstance()
{
    static Client client;
    return client;
}

Client::Client() {}

void Client::Start()
{
    _send_msg = MsgSender::GetInstance();
    _recv_msg = MsgReceiver::GetInstance();
    _send_msg->SetSocket(_sock);
    _recv_msg->SetSocket(_sock);
    _recv_msg->start();
}

void Client::connectServer()
{
    // 创建上下文服务
    boost::asio::io_context ioc;
    //构造endpoint
    tcp::endpoint remote_ep(make_address(ipaddress), PORT);
    _sock = new tcp::socket(ioc);
    (*_sock).connect(remote_ep);

    _logic_thread = std::thread(&Client::Start, this);
}

void Client::setIpAddress(std::string address)
{
    ipaddress = address;
}
