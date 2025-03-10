#undef signals
#include "piplinebuild.h"
#define signals Q_SIGNALS

#include <QDebug>
#include "ConstValue.h"
#include "filetools.h"
#include "recvmsg.h"
#include "user.h"
#include <iostream>
#include <nlohmann/json.hpp>

using namespace nlohmann;

RecvMsg::~RecvMsg() {}

RecvMsg::RecvMsg() {}

void RecvMsg::Start()
{
    _recv_thread = std::thread(&RecvMsg::DealMsg, this);
    RegisterCallBacks();
    ReceiveMsg();
}

void RecvMsg::SetSocket(boost::asio::ip::tcp::socket *sock)
{
    _sock = sock;
}

void RecvMsg::ReceiveMsg()
{
    for (;;) {
        char head[HEAD_TOTAL_LENGTH];
        boost::asio::read(*_sock, boost::asio::buffer(head, HEAD_TOTAL_LENGTH));
        short msg_id;
        memcpy(&msg_id, head, HEAD_ID_LENGTH);
        msg_id = boost::asio::detail::socket_ops::network_to_host_short(msg_id);

        short msg_len;
        memcpy(&msg_len, head + 2, HEAD_DATA_LENGTH);
        msg_len = boost::asio::detail::socket_ops::network_to_host_short(msg_len);

        char msg_data[msg_len];
        memset(msg_data, 0, msg_len);

        boost::asio::read(*_sock, boost::asio::buffer(msg_data, msg_len));

        std::shared_ptr<RecvNode> recv_node = std::make_shared<RecvNode>(msg_len, msg_id);
        memcpy(recv_node->_data, msg_data, msg_len);
        _recv_que.push(recv_node);

        _consume.notify_one();

        std::cout << "receive-----------------------------------------" << std::endl;
        std::cout << "msg_id:" << msg_id << std::endl;
        std::cout << "msg_len:" << msg_len << std::endl;
        std::cout << "head:" << head << std::endl;

        std::cout << "msg_data:" << msg_data << std::endl;
        std::cout << "recvnode_data:" << recv_node->_data << std::endl;
    }
}

void RecvMsg::DealMsg()
{
    for (;;) {
        std::cout << "DealMsg-----------------------------------\n";
        std::unique_lock<std::mutex> unique_lk(_mutex);

        // 队列为空，则用条件变量等待（挂起线程，加锁解锁需要消耗cpu资源）
        while (_recv_que.empty() && !_b_stop) {
            _consume.wait(unique_lk); // 线程挂起，释放资源，解锁，等待
        }

        // 客户端即将退出
        if (_b_stop) {
            while (!_recv_que.empty()) {
                auto msg_node = _recv_que.front();
                const short msg_id = msg_node->GetMsgId();
                auto call_back_iter = _fun_callbacks.find(msg_id);
                if (call_back_iter == _fun_callbacks.end()) {
                    _recv_que.pop();
                    continue;
                }
                call_back_iter->second(
                    std::string(msg_node->_data, msg_node->_total_len - HEAD_TOTAL_LENGTH));
                _recv_que.pop();
            }
            return;
        }

        auto msg_node = _recv_que.front();
        const short msg_id = msg_node->GetMsgId();
        auto call_back_iter = _fun_callbacks.find(msg_id);
        if (call_back_iter == _fun_callbacks.end()) {
            _recv_que.pop();
            return;
        }
        call_back_iter->second(
            std::string(msg_node->_data, msg_node->_total_len - HEAD_TOTAL_LENGTH));
        _recv_que.pop();
    }
}

void RecvMsg::RegisterCallBacks()
{
    _fun_callbacks[MSG_HELLO_WORLD] = std::bind(&RecvMsg::HelloWorldCallBack,
                                                this,
                                                std::placeholders::_1);

    _fun_callbacks[MSG_NOT_ONLINE] = std::bind(&RecvMsg::NotOnlineCallBack,
                                               this,
                                               std::placeholders::_1);

    _fun_callbacks[MSG_USER_INFO] = std::bind(&RecvMsg::UserInfoCallBack,
                                              this,
                                              std::placeholders::_1);

    _fun_callbacks[MSG_GET_FOLLOWINGS] = std::bind(&RecvMsg::GetFollowingCallBack,
                                                   this,
                                                   std::placeholders::_1);

    _fun_callbacks[MSG_GET_FOLLOWERS] = std::bind(&RecvMsg::GetFollowerCallBack,
                                                  this,
                                                  std::placeholders::_1);

    _fun_callbacks[MSG_GET_BLACKLIST] = std::bind(&RecvMsg::GetBlacklistCallBack,
                                                  this,
                                                  std::placeholders::_1);

    _fun_callbacks[MSG_TEXT_CHAT] = std::bind(&RecvMsg::TextChatCallBack,
                                              this,
                                              std::placeholders::_1);

    _fun_callbacks[MSG_TEXT_CHAT_REFUSED] = std::bind(&RecvMsg::TextChatRefusedCallBack,
                                                      this,
                                                      std::placeholders::_1);

    _fun_callbacks[MSG_FOLLOWING] = std::bind(&RecvMsg::FollowingCallBack,
                                              this,
                                              std::placeholders::_1);

    _fun_callbacks[MSG_CANCEL_FOLLOW] = std::bind(&RecvMsg::CancelFollowCallBack,
                                                  this,
                                                  std::placeholders::_1);

    _fun_callbacks[MSG_VIDEO_CHAT] = std::bind(&RecvMsg::VideoChatCallBack,
                                               this,
                                               std::placeholders::_1);

    _fun_callbacks[MSG_AGREE_VIDEO] = std::bind(&RecvMsg::AgreeVideoChatCallBack,
                                                this,
                                                std::placeholders::_1);

    _fun_callbacks[MSG_VIDEO_CHAT_REFUSED] = std::bind(&RecvMsg::RefusedVideoChatCallBack,
                                                       this,
                                                       std::placeholders::_1);

    _fun_callbacks[MSG_AUDIO_CHAT] = std::bind(&RecvMsg::AudioChatCallBack,
                                               this,
                                               std::placeholders::_1);

    _fun_callbacks[MSG_AGREE_AUDIO] = std::bind(&RecvMsg::AgreeAudioChatCallBack,
                                                this,
                                                std::placeholders::_1);

    _fun_callbacks[MSG_AUDIO_CHAT_REFUSED] = std::bind(&RecvMsg::RefusedAudioChatCallBack,
                                                       this,
                                                       std::placeholders::_1);

    _fun_callbacks[MSG_RANDOM_PUSH] = std::bind(&RecvMsg::RandomPushCallBack,
                                                this,
                                                std::placeholders::_1);
}

void RecvMsg::HelloWorldCallBack(const std::string &msg_data)
{
    qDebug() << "HelloWorldCallBack----------------";
}

void RecvMsg::NotOnlineCallBack(const std::string &msg_data)
{
    qDebug() << "NotOnlineCallBack----------------";

    // 发信号给前端提醒对方不在线
}

void RecvMsg::UserInfoCallBack(const std::string &msg_data)
{
    qDebug() << "UserInfoCallBack----------------" << msg_data;

    json jsonmsg = json::parse(msg_data);
    jsonmsg = jsonmsg["data"];

    FileTools::GetInstance()->SaveUserInfo(jsonmsg);
    User::GetInstance()->SetMyInfo(jsonmsg);
}

void RecvMsg::GetFollowingCallBack(const std::string &msg_data)
{
    qDebug() << "GetFollowingCallBack----------------";

    json jsonmsg = json::parse(msg_data);
    jsonmsg = jsonmsg["data"];

    for (const auto &item : jsonmsg) {
        std::string str = item["uid"];
        FileTools::GetInstance()->SaveRelation(RELATION_FOLLOWING, std::stoi(str), item);
        User::GetInstance()->InsertToFollowing(std::stoi(str), item);
    }
}

void RecvMsg::GetFollowerCallBack(const std::string &msg_data)
{
    qDebug() << "GetFollowerCallBack----------------";

    json jsonmsg = json::parse(msg_data);
    jsonmsg = jsonmsg["data"];

    for (const auto &item : jsonmsg) {
        std::string str = item["uid"];

        FileTools::GetInstance()->SaveRelation(RELATION_FOLLOWER, std::stoi(str), item);
        User::GetInstance()->InsertToFollower(std::stoi(str), item);
    }
}

void RecvMsg::GetBlacklistCallBack(const std::string &msg_data)
{
    qDebug() << "GetBlacklistCallBack----------------";

    json jsonmsg = json::parse(msg_data);
    jsonmsg = jsonmsg["data"];

    for (const auto &item : jsonmsg) {
        std::string str = item["uid"];

        FileTools::GetInstance()->SaveRelation(RELATION_BLOCK, std::stoi(str), item);
        User::GetInstance()->InsertToBlacklist(std::stoi(str), item);
    }
}

void RecvMsg::TextChatCallBack(const std::string &msg_data)
{
    qDebug() << "TextChatCallBack----------------";

    json jsonmsg = json::parse(msg_data);
    std::cout << "jsonmsg:" << jsonmsg << std::endl;
    unsigned int object_id = jsonmsg["object_id"];
    FileTools::GetInstance()->SaveTextMsg(object_id, jsonmsg);
    FileTools::GetInstance()->GetLatestMsg(object_id);
}

void RecvMsg::TextChatRefusedCallBack(const std::string &msg_data)
{
    qDebug() << "TextChatRefusedCallBack----------------";

    // 发信号给前端：你现在不能给对方发送消息，因为对方尚未回复/对方将你拉黑/你已拉黑对方
}

void RecvMsg::FollowingCallBack(const std::string &msg_data)
{
    qDebug() << "FollowingCallBack----------------";

    json jsonmsg = json::parse(msg_data);
    std::cout << "jsonmsg:" << jsonmsg << std::endl;
    unsigned int object_id = jsonmsg["object_id"];

    User::GetInstance()->InsertToFollower(object_id, jsonmsg);
    FileTools::GetInstance()->SaveRelation(RELATION_FOLLOWER, object_id, jsonmsg);

    // 发个信号给通知前端有新的关注者
}

void RecvMsg::CancelFollowCallBack(const std::string &msg_data)
{
    qDebug() << "CancelFollowCallBack----------------";

    json jsonmsg = json::parse(msg_data);
    std::cout << "jsonmsg:" << jsonmsg << std::endl;
    unsigned int object_id = jsonmsg["object_id"];

    User::GetInstance()->RemoveFromFollower(object_id);
    FileTools::GetInstance()->RemoveRelation(RELATION_FOLLOWER, object_id);

    // 发个信号给通知前端失去一个关注者
}

void RecvMsg::VideoChatCallBack(const std::string &msg_data)
{
    qDebug() << "VideoChatCallBack----------------";

    auto jsonmsg = json::parse(msg_data);
    std::string data = jsonmsg["data"];
    const gchar *temp = data.c_str();
    gchar *text = g_strdup(temp);
    JsonNode *root;
    JsonObject *object, *child;
    JsonParser *parser = json_parser_new();
    if (!json_parser_load_from_data(parser, text, -1, NULL)) {
        gst_printerr("json object get failed from text:%s\n", text);
        g_object_unref(parser);
        g_free(text);
    }
    root = json_parser_get_root(parser);
    if (!JSON_NODE_HOLDS_OBJECT(root)) {
        gst_printerr("json object get failed 2 from text:%s\n", text);
        g_object_unref(parser);
        g_free(text);
    }
    object = json_node_get_object(root);
    if (json_object_has_member(object, "sdp")) {
        int ret;
        GstSDPMessage *sdp;
        const gchar *sdptext, *sdptype;
        GstWebRTCSessionDescription *answer;

        PiplineBuild::app_state = PEER_CALL_NEGOTIATING;
        child = json_object_get_object_member(object, "sdp");

        sdptype = json_object_get_string_member(child, "type");
        sdptext = json_object_get_string_member(child, "sdp");
        ret = gst_sdp_message_new(&sdp);
        if (!ret) {
            gst_printerr("create GSTSDPMessage failed");
        }
        ret = gst_sdp_message_parse_buffer((guint8 *) sdptext, strlen(sdptext), sdp);
        if (!ret) {
            gst_printerr("parse sdptext failed");
        }

        //受到的sdp类型是answer,设置本端的远程描述，将状态改为开始通信
        if (g_str_equal(sdptype, "answer")) {
            gst_print("Recived answer:\n%s\n", sdptext);
            answer = gst_webrtc_session_description_new(GST_WEBRTC_SDP_TYPE_ANSWER, sdp);
            g_assert_nonnull(answer);

            GstPromise *promise = gst_promise_new();
            g_signal_emit_by_name(PiplineBuild::m_webrtcbin,
                                  "set-remote-description",
                                  answer,
                                  promise);
            gst_promise_interrupt(promise);
            gst_promise_unref(promise);

            PiplineBuild::app_state = PEER_CALL_STARTED;
        }
        //收到的sdp类型是offer,开始创建自己的管道，
        //将收到的sdp设置为远端描述，创建answer通过服务器发送给对端
        else {
            gst_print("Receive offer:\n%s\n", sdptext);
            PiplineBuild::on_offer_received(sdp);
        }
    } else if (json_object_has_member(object, "ice")) {
        const gchar *candidate;
        gint sdpmlineindex;

        child = json_object_get_object_member(object, "ice");
        candidate = json_object_get_string_member(child, "candidate");
        sdpmlineindex = json_object_get_int_member(child, "sdpMLineIndex");

        g_signal_emit_by_name(PiplineBuild::m_webrtcbin,
                              "add-ice-candidate",
                              sdpmlineindex,
                              candidate);
    } else {
        g_print("other json msg");
    }
    g_object_unref(parser);
}

void RecvMsg::AgreeVideoChatCallBack(const std::string &msg_data)
{
    qDebug() << "AgreeVideoChatCallBack----------------";

    // 发信号给前端：视频通话请求已同意
}

void RecvMsg::RefusedVideoChatCallBack(const std::string &msg_data)
{
    qDebug() << "RefusedVideoChatCallBack----------------";

    // 发信号给前端：视频通话请求被拒绝

    // 断开管道连接
}

void RecvMsg::AudioChatCallBack(const std::string &msg_data)
{
    qDebug() << "AudioChatCallBack----------------";
    // 发信号给前端：视频通话请求已同意
}

void RecvMsg::AgreeAudioChatCallBack(const std::string &msg_data)
{
    qDebug() << "AgreeAudioChatCallBack----------------";
    // 发信号给前端：视频通话请求已同意
}

void RecvMsg::RefusedAudioChatCallBack(const std::string &msg_data)
{
    qDebug() << "RefusedAudioChatCallBack----------------";
    // 发信号给前端：视频通话请求被拒绝

    // 断开管道连接
}

void RecvMsg::RandomPushCallBack(const std::string &msg_data)
{
    qDebug() << "RandomPushCallBack----------------";

    json jsonmsg = json::parse(msg_data);
    std::cout << "jsonmsg:" << jsonmsg << std::endl;
    json users_data = jsonmsg["data"];
    // users_data是json数组，把数据转给前端
}
