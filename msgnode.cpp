#include "msgnode.h"
SendNode::SendNode(const char* data, short max_len, unsigned int object_id, short msg_id)
    : MsgNode(max_len)
    , _object_id(object_id)
    , _msg_id(msg_id)
{
    // 将 msg_id 和 max_len 转换为网络字节序（大端序）
    short msg_id_network = boost::asio::detail::socket_ops::host_to_network_short(msg_id);
    short max_len_network = boost::asio::detail::socket_ops::host_to_network_short(max_len);

    memcpy(_data, &msg_id_network, HEAD_ID_LENGTH);
    memcpy(_data + HEAD_ID_LENGTH, &max_len_network, HEAD_DATA_LENGTH);
    memcpy(_data + HEAD_ID_LENGTH + HEAD_DATA_LENGTH, data, max_len);
}

RecvNode::RecvNode(short max_len, short msg_id)
    : MsgNode(max_len)
    , _msg_id(msg_id)
{}
