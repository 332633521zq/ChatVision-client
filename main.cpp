// #include <gst/gst.h>
// #include <gst/sdp/sdp.h>
// #include <gst/webrtc/webrtc.h>

// #include <string.h>

// static GMainLoop *loop;
// static GstElement *pipe1, *webrtc1, *webrtc2;
// static GstBus *bus1;

// static gboolean _bus_watch(GstBus *bus, GstMessage *msg, GstElement *pipe)
// {
//     switch (GST_MESSAGE_TYPE(msg)) {
//     case GST_MESSAGE_STATE_CHANGED:
//         if (GST_ELEMENT(msg->src) == pipe) {
//             GstState old;
//             GstState new1;
//             GstState pending;

//             gst_message_parse_state_changed(msg, &old, &new1, &pending);

//             {
//                 gchar *dump_name = g_strconcat("state_changed-",
//                                                gst_element_state_get_name(old),
//                                                "_",
//                                                gst_element_state_get_name(new1),
//                                                NULL);
//                 GST_DEBUG_BIN_TO_DOT_FILE_WITH_TS(GST_BIN(msg->src),
//                                                   GST_DEBUG_GRAPH_SHOW_ALL,
//                                                   dump_name);
//                 g_free(dump_name);
//             }
//         }
//         break;
//     case GST_MESSAGE_ERROR: {
//         GError *err = NULL;
//         gchar *dbg_info = NULL;

//         GST_DEBUG_BIN_TO_DOT_FILE_WITH_TS(GST_BIN(pipe), GST_DEBUG_GRAPH_SHOW_ALL, "error");

//         gst_message_parse_error(msg, &err, &dbg_info);
//         g_printerr("ERROR from element %s: %s\n", GST_OBJECT_NAME(msg->src), err->message);
//         g_printerr("Debugging info: %s\n", (dbg_info) ? dbg_info : "none");
//         g_error_free(err);
//         g_free(dbg_info);
//         g_main_loop_quit(loop);
//         break;
//     }
//     case GST_MESSAGE_EOS: {
//         GST_DEBUG_BIN_TO_DOT_FILE_WITH_TS(GST_BIN(pipe), GST_DEBUG_GRAPH_SHOW_ALL, "eos");
//         g_print("EOS received\n");
//         g_main_loop_quit(loop);
//         break;
//     }
//     default:
//         break;
//     }

//     return TRUE;
// }

// static void _webrtc_pad_added(GstElement *webrtc, GstPad *new_pad, GstElement *pipe)
// {
//     GstElement *out;
//     GstPad *sink;

//     if (GST_PAD_DIRECTION(new_pad) != GST_PAD_SRC)
//         return;

//     out = gst_parse_bin_from_description("rtpvp8depay ! vp8dec ! "
//                                          "videoconvert ! queue ! xvimagesink",
//                                          TRUE,
//                                          NULL);
//     gst_bin_add(GST_BIN(pipe), out);
//     gst_element_sync_state_with_parent(out);

//     sink = GST_PAD_CAST(out->sinkpads->data);

//     gst_pad_link(new_pad, sink);
// }

// static void _on_answer_received(GstPromise *promise, gpointer user_data)
// {
//     GstWebRTCSessionDescription *answer = NULL;
//     const GstStructure *reply;
//     gchar *desc;

//     g_assert(gst_promise_wait(promise) == GST_PROMISE_RESULT_REPLIED);
//     reply = gst_promise_get_reply(promise);
//     gst_structure_get(reply, "answer", GST_TYPE_WEBRTC_SESSION_DESCRIPTION, &answer, NULL);
//     gst_promise_unref(promise);
//     desc = gst_sdp_message_as_text(answer->sdp);
//     g_print("Created answer:\n%s\n", desc);
//     g_free(desc);

//     /* this is one way to tell webrtcbin that we don't want to be notified when
//      * this task is complete: set a NULL promise */
//     g_signal_emit_by_name(webrtc1, "set-remote-description", answer, NULL);
//     /* this is another way to tell webrtcbin that we don't want to be notified
//      * when this task is complete: interrupt the promise */
//     promise = gst_promise_new();
//     g_signal_emit_by_name(webrtc2, "set-local-description", answer, promise);
//     gst_promise_interrupt(promise);
//     gst_promise_unref(promise);

//     gst_webrtc_session_description_free(answer);
// }

// static void _on_offer_received(GstPromise *promise, gpointer user_data)
// {
//     GstWebRTCSessionDescription *offer = NULL;
//     const GstStructure *reply;
//     gchar *desc;

//     g_assert(gst_promise_wait(promise) == GST_PROMISE_RESULT_REPLIED);
//     reply = gst_promise_get_reply(promise);
//     gst_structure_get(reply, "offer", GST_TYPE_WEBRTC_SESSION_DESCRIPTION, &offer, NULL);
//     gst_promise_unref(promise);
//     desc = gst_sdp_message_as_text(offer->sdp);
//     g_print("Created offer:\n%s\n", desc);
//     g_free(desc);

//     g_signal_emit_by_name(webrtc1, "set-local-description", offer, NULL);
//     g_signal_emit_by_name(webrtc2, "set-remote-description", offer, NULL);

//     promise = gst_promise_new_with_change_func(_on_answer_received, user_data, NULL);
//     g_signal_emit_by_name(webrtc2, "create-answer", NULL, promise);

//     gst_webrtc_session_description_free(offer);
// }

// static void _on_negotiation_needed(GstElement *element, gpointer user_data)
// {
//     GstPromise *promise;

//     promise = gst_promise_new_with_change_func(_on_offer_received, user_data, NULL);
//     g_signal_emit_by_name(webrtc1, "create-offer", NULL, promise);
// }

// static void _on_ice_candidate(GstElement *webrtc,
//                               guint mlineindex,
//                               gchar *candidate,
//                               GstElement *other)
// {
//     g_signal_emit_by_name(other, "add-ice-candidate", mlineindex, candidate);
// }

// int main(int argc, char *argv[])
// {
//     gst_init(&argc, &argv);

//     loop = g_main_loop_new(NULL, FALSE);
//     pipe1 = gst_parse_launch(
//         "v4l2src ! queue ! videoconvert ! vp8enc ! rtpvp8pay ! queue ! "
//         "application/x-rtp,media=video,payload=96,encoding-name=VP8 ! "
//         "webrtcbin name=smpte videotestsrc pattern=ball ! queue ! vp8enc ! rtpvp8pay ! queue ! "
//         "application/x-rtp,media=video,payload=96,encoding-name=VP8 ! webrtcbin name=ball",
//         NULL);
//     bus1 = gst_pipeline_get_bus(GST_PIPELINE(pipe1));
//     gst_bus_add_watch(bus1, (GstBusFunc) _bus_watch, pipe1);

//     webrtc1 = gst_bin_get_by_name(GST_BIN(pipe1), "smpte");
//     g_signal_connect(webrtc1, "on-negotiation-needed", G_CALLBACK(_on_negotiation_needed), NULL);
//     g_signal_connect(webrtc1, "pad-added", G_CALLBACK(_webrtc_pad_added), pipe1);
//     webrtc2 = gst_bin_get_by_name(GST_BIN(pipe1), "ball");
//     g_signal_connect(webrtc2, "pad-added", G_CALLBACK(_webrtc_pad_added), pipe1);
//     g_signal_connect(webrtc1, "on-ice-candidate", G_CALLBACK(_on_ice_candidate), webrtc2);
//     g_signal_connect(webrtc2, "on-ice-candidate", G_CALLBACK(_on_ice_candidate), webrtc1);

//     g_print("Starting pipeline\n");
//     gst_element_set_state(GST_ELEMENT(pipe1), GST_STATE_PLAYING);

//     g_main_loop_run(loop);

//     gst_element_set_state(GST_ELEMENT(pipe1), GST_STATE_NULL);
//     g_print("Pipeline stopped\n");

//     gst_object_unref(webrtc1);
//     gst_object_unref(webrtc2);
//     gst_bus_remove_watch(bus1);
//     gst_object_unref(bus1);
//     gst_object_unref(pipe1);

//     gst_deinit();

//     return 0;
// }

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include "ConstValue.h"
#include "mediathread.h"
#include <boost/asio.hpp>
#include <iostream>
#include <json/json.h>
#include <json/reader.h>
#include <json/value.h>
#include <nlohmann/json.hpp>
#include <thread>

using namespace std;
using namespace boost::asio::ip;
using namespace nlohmann;

// #define IPADDRESS "192.168.253.186"
#define IPADDRESS "127.0.0.1"
#define PORT 10086

// const int MAX_LENGTH = 1024 * 2;
const int HEAD_LENGTH = 2;
const int HEAD_TOTAL = 4;

// unsigned int uid = 20000001;
extern unsigned int uid;
unsigned int object_id = 20000001;

void SendRequest(tcp::socket& sock, char* data, unsigned int object_id, unsigned int msg_id);
void SendRequest(tcp::socket& sock, std::string data, unsigned int object_id, unsigned int msg_id);

int main(int argc, char* argv[])
{
    try {
        //创建上下文服务
        boost::asio::io_context ioc;
        //构造endpoint
        tcp::endpoint remote_ep(make_address(IPADDRESS), PORT);
        tcp::socket sock(ioc);
        boost::system::error_code error = boost::asio::error::host_not_found;
        sock.connect(remote_ep, error);
        if (error) {
            cout << "connect failed, code is " << error.value() << " error msg is "
                 << error.message();
            return 0;
        }

        // Json::Value root;
        // root["id"] = 1001;
        // root["data"] =
#define IPADDRESS "192.168.253.186"
        thread send_thread([&sock] {
            // for (;;) {
            this_thread::sleep_for(std::chrono::milliseconds(2000));
            std::string request = "hello world";
            std::string null_str = "";
            std::string chat_text = "hello friend, my uid is: " + std::to_string(uid)
                                    + ",nice to meet you";
            // SendRequest(sock, request, uid, MSG_HELLO_WORLD);
            SendRequest(sock, null_str, uid, MSG_LOGIN);
            // SendRequest(sock, chat_text, object_id, MSG_TEXT_CHAT);
            // }
        });

        thread recv_thread([&sock] {
            for (;;) {
                this_thread::sleep_for(std::chrono::milliseconds(2000));
                cout << "begin to receive..." << endl;

                char reply_head[HEAD_TOTAL];
                size_t reply_length = boost::asio::read(sock,
                                                        boost::asio::buffer(reply_head, HEAD_TOTAL));

                cout << "receive finished" << endl;

                unsigned int msgid = 0;
                memcpy(&msgid, reply_head, HEAD_LENGTH);
                short msglen = 0;
                memcpy(&msglen, reply_head + 2, HEAD_LENGTH);
                //转为本地字节序
                msglen = boost::asio::detail::socket_ops::network_to_host_short(msglen);
                msgid = boost::asio::detail::socket_ops::network_to_host_short(msgid);
                char msg[MAX_LENGTH] = {0};
                size_t msg_length = boost::asio::read(sock, boost::asio::buffer(msg, msglen));
                // std::cout << "msg: " << msg << std::endl;

                json msg_data = json::parse(msg);
                std::cout << "msg id is " << msgid << "\nreceived msg length is " << msg_length
                          << endl;
                std::cout << "my uid is: " << msg_data["uid"] << std::endl;
                std::cout << "object id is: " << msg_data["object_id"] << std::endl;
                std::cout << "msg_data is: " << msg_data["data"] << std::endl;

                /***************************zj***********************/
                std::string data = msg_data["data"];
                const gchar* temp = data.c_str();
                gchar* text = g_strdup(temp);
                JsonNode* root;
                JsonObject *object, *child;
                JsonParser* parser = json_parser_new();
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
                    GstSDPMessage* sdp;
                    const gchar *sdptext, *sdptype;
                    GstWebRTCSessionDescription* answer;

                    PiplineBuild::app_state = PEER_CALL_NEGOTIATING;
                    child = json_object_get_object_member(object, "sdp");

                    sdptype = json_object_get_string_member(child, "type");
                    sdptext = json_object_get_string_member(child, "sdp");
                    ret = gst_sdp_message_new(&sdp);
                    if (!ret) {
                        gst_printerr("create GSTSDPMessage failed");
                    }
                    ret = gst_sdp_message_parse_buffer((guint8*) sdptext, strlen(sdptext), sdp);
                    if (!ret) {
                        gst_printerr("parse sdptext failed");
                    }

                    //受到的sdp类型是answer,设置本端的远程描述，将状态改为开始通信
                    if (g_str_equal(sdptype, "answer")) {
                        gst_print("Recived answer:\n%s\n", sdptext);
                        answer = gst_webrtc_session_description_new(GST_WEBRTC_SDP_TYPE_ANSWER, sdp);
                        g_assert_nonnull(answer);

                        GstPromise* promise = gst_promise_new();
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
                    const gchar* candidate;
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
        });
        // getchar();
        QGuiApplication app(argc, argv);

        gst_init(&argc, &argv);

        sleep(5);
        //媒体线程
        PiplineBuild::m_socket = &sock;
        PiplineBuild::m_object_id = object_id;
        MediaThread* mediaThread = new MediaThread();
        mediaThread->startThread();

        QQmlApplicationEngine engine;
        const QUrl url(QStringLiteral("qrc:/Multimedia-moudle-Gstreamer/Main.qml"));
        QObject::connect(
            &engine,
            &QQmlApplicationEngine::objectCreationFailed,
            &app,
            []() { QCoreApplication::exit(-1); },
            Qt::QueuedConnection);
        engine.load(url);

        return app.exec();

    } catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << endl;
    }
}
void SendRequest(tcp::socket& sock, std::string data, unsigned int object_id, unsigned int msg_id)
{
    char send_data[MAX_LENGTH] = {0};
    int msgid_host = boost::asio::detail::socket_ops::host_to_network_short(msg_id);
    memcpy(send_data, &msgid_host, 2);

    json send_str;
    send_str["uid"] = uid;
    send_str["object_id"] = object_id;
    send_str["data"] = data;
    std::string temp_send_str = send_str.dump();

    int request_host_length = boost::asio::detail::socket_ops::host_to_network_short(
        temp_send_str.size());
    memcpy(send_data + 2, &request_host_length, 2);
    memcpy(send_data + 4, temp_send_str.c_str(), temp_send_str.size());
    std::cout << "send_data: " << send_str.dump() << "length:" << sizeof(send_str.dump()) + 1
              << std::endl;
    boost::asio::write(sock, boost::asio::buffer(send_data, temp_send_str.size() + 4));
}

void SendRequest(char* data, unsigned int msg_id) {}
