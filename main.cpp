#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "client.h"
#include "communicationpagecontroller.h"
#include "personalpagecontroller.h"
// #include "ConstValue.h"
// #include "mediathread.h"
// #include <boost/asio.hpp>
// #include <iostream>
// #include <nlohmann/json.hpp>
// #include <thread>

using namespace boost::asio::ip;

// #define IPADDRESS "192.168.253.186"
#define IPADDRESS "127.0.0.1"
#define PORT 10086

int main(int argc, char* argv[])
{
    // // 创建上下文服务
    // boost::asio::io_context ioc;
    // //构造endpoint
    // tcp::endpoint remote_ep(make_address(IPADDRESS), PORT);
    // tcp::socket sock(ioc);
    // // sock = new tcp::socket(ioc);
    // sock.connect(remote_ep);

    Client client;
    //             /***************************zj***********************/
    //             std::string data = msg_data["data"];
    //             const gchar* temp = data.c_str();
    //             gchar* text = g_strdup(temp);
    //             JsonNode* root;
    //             JsonObject *object, *child;
    //             JsonParser* parser = json_parser_new();
    //             if (!json_parser_load_from_data(parser, text, -1, NULL)) {
    //                 gst_printerr("json object get failed from text:%s\n", text);
    //                 g_object_unref(parser);
    //                 g_free(text);
    //             }
    //             root = json_parser_get_root(parser);
    //             if (!JSON_NODE_HOLDS_OBJECT(root)) {
    //                 gst_printerr("json object get failed 2 from text:%s\n", text);
    //                 g_object_unref(parser);
    //                 g_free(text);
    //             }
    //             object = json_node_get_object(root);
    //             if (json_object_has_member(object, "sdp")) {
    //                 int ret;
    //                 GstSDPMessage* sdp;
    //                 const gchar *sdptext, *sdptype;
    //                 GstWebRTCSessionDescription* answer;

    //                 PiplineBuild::app_state = PEER_CALL_NEGOTIATING;
    //                 child = json_object_get_object_member(object, "sdp");

    //                 sdptype = json_object_get_string_member(child, "type");
    //                 sdptext = json_object_get_string_member(child, "sdp");
    //                 ret = gst_sdp_message_new(&sdp);
    //                 if (!ret) {
    //                     gst_printerr("create GSTSDPMessage failed");
    //                 }
    //                 ret = gst_sdp_message_parse_buffer((guint8*) sdptext, strlen(sdptext), sdp);
    //                 if (!ret) {
    //                     gst_printerr("parse sdptext failed");
    //                 }

    //                 //受到的sdp类型是answer,设置本端的远程描述，将状态改为开始通信
    //                 if (g_str_equal(sdptype, "answer")) {
    //                     gst_print("Recived answer:\n%s\n", sdptext);
    //                     answer = gst_webrtc_session_description_new(GST_WEBRTC_SDP_TYPE_ANSWER, sdp);
    //                     g_assert_nonnull(answer);

    //                     GstPromise* promise = gst_promise_new();
    //                     g_signal_emit_by_name(PiplineBuild::m_webrtcbin,
    //                                           "set-remote-description",
    //                                           answer,
    //                                           promise);
    //                     gst_promise_interrupt(promise);
    //                     gst_promise_unref(promise);

    //                     PiplineBuild::app_state = PEER_CALL_STARTED;
    //                 }
    //                 //收到的sdp类型是offer,开始创建自己的管道，
    //                 //将收到的sdp设置为远端描述，创建answer通过服务器发送给对端
    //                 else {
    //                     gst_print("Receive offer:\n%s\n", sdptext);
    //                     PiplineBuild::on_offer_received(sdp);
    //                 }
    //             } else if (json_object_has_member(object, "ice")) {
    //                 const gchar* candidate;
    //                 gint sdpmlineindex;

    //                 child = json_object_get_object_member(object, "ice");
    //                 candidate = json_object_get_string_member(child, "candidate");
    //                 sdpmlineindex = json_object_get_int_member(child, "sdpMLineIndex");

    //                 g_signal_emit_by_name(PiplineBuild::m_webrtcbin,
    //                                       "add-ice-candidate",
    //                                       sdpmlineindex,
    //                                       candidate);
    //             } else {
    //                 g_print("other json msg");
    //             }
    //             g_object_unref(parser);
    //         }

    QGuiApplication app(argc, argv);

    // gst_init(&argc, &argv);

    // sleep(3);
    // //媒体线程
    // PiplineBuild::m_socket = &sock;
    // PiplineBuild::m_object_id = object_id;
    // MediaThread* mediaThread = new MediaThread();
    // mediaThread->startThread();

    QQmlApplicationEngine engine;

    engine.rootContext()->setContextProperty("personalPageControler",
                                             &PersonalPageController::getInstance());
    engine.rootContext()->setContextProperty("communicationPageControler",
                                             &CommunicationPageController::getInstance());

    const QUrl url(QStringLiteral("qrc:/ChatVision-client/LoginPage.qml"));
    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
