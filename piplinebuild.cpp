//这个版本的管道设计没问题，可以向anser发起通话，视频流正常，音频流耳机正常，系统内置音频有杂音。
//添加了loop循环，但好像没有其作用，bus总线上的watch好像也没有其作用，因为我点击关闭视频窗口没有错误弹出
//还需要考虑关闭一个视频通话后，与另一个用户建立连接的话怎么创新创建管道，管道的这些元素都是静态的，注意释放和重新赋值
//现在先去搭建ui界面，后面再考虑这部分代码的调整

#include "piplinebuild.h"
#include "sendmsg.h"

unsigned int uid = 20000000;

GstElement *PiplineBuild::m_audio_bin = nullptr;
GstElement *PiplineBuild::m_pipeline = nullptr;
GstElement *PiplineBuild::m_video_bin = nullptr;
GstElement *PiplineBuild::m_webrtcbin = nullptr;
GObject *PiplineBuild::receive_channel = nullptr;
GObject *PiplineBuild::send_channel = nullptr;
GMainLoop *PiplineBuild::loop = g_main_loop_new(NULL, FALSE);
enum AppState PiplineBuild::app_state = APP_STATE_UNKNOWN;
gboolean PiplineBuild::is_offer = FALSE;

boost::asio::ip::tcp::socket *PiplineBuild::m_socket = nullptr;
unsigned int PiplineBuild::m_object_id = NULL;

PiplineBuild::PiplineBuild() {}

gboolean PiplineBuild::start_pipeline(gboolean create_offer)
{
    GstBus *bus;

    //推流时的视频箱和音频箱描述
    char *audio_desc, *video_desc;

    //开启管道时检查的管道状态改变返回值
    GstStateChangeReturn ret;

    GError *audio_erro = NULL;
    GError *video_erro = NULL;

    //创建管道
    m_pipeline = gst_pipeline_new("webrtc-pipeline");

    //创建音频箱
    audio_desc = g_strdup_printf(
        "autoaudiosrc ! audioconvert ! audio/x-raw,format=S16LE ! audioresample ! "
        "audio/x-raw,rate=48000 ! volume volume=0.5 ! queue ! "
        "capsfilter caps=audio/x-raw,rate=48000 ! opusenc "
        "perfect-timestamp=true bitrate=64000 ! rtpopuspay name=audiopay pt=%u "
        "! application/x-rtp, media=audio, encoding-name=OPUS, payload=%u, clock-rate=48000 ! "
        "queue",
        RTP_OPUS_DEFAULT_PT,
        RTP_OPUS_DEFAULT_PT);
    m_audio_bin = gst_parse_bin_from_description(audio_desc, TRUE, &audio_erro);
    g_free(audio_desc);
    if (audio_erro) {
        gst_printerr("Failed to parse m_audio_bin from audio_desc:%s\n", audio_erro->message);
        g_error_free(audio_erro);
        if (m_pipeline) {
            g_clear_object(&m_pipeline);
        }
        if (m_webrtcbin) {
            m_webrtcbin = NULL;
        }
        return FALSE;
    }

    //创建视频箱
    video_desc = g_strdup_printf("v4l2src ! videoconvert ! queue ! vp8enc "
                                 "deadline=1 keyframe-max-dist=2000 ! "
                                 "rtpvp8pay name=videopay picture-id-mode=15-bit pt=%u ! queue",
                                 RTP_VP8_DEFAULT_PT);
    m_video_bin = gst_parse_bin_from_description(video_desc, TRUE, &video_erro);
    g_free(video_desc);
    if (video_erro) {
        gst_printerr("Failed to parse m_video_bin from video_desc:%s\n", video_erro->message);
        g_error_free(video_erro);
        if (m_pipeline) {
            g_clear_object(&m_pipeline);
        }
        if (m_webrtcbin) {
            m_webrtcbin = NULL;
        }
        return FALSE;
    }

    //创建webrtcbin元素并设置stun服务器
    m_webrtcbin = gst_element_factory_make_full("webrtcbin",
                                                "name",
                                                "sendrecv",
                                                "stun-server",
                                                STUN_SERVER,
                                                NULL);
    g_assert_nonnull(m_webrtcbin);
    g_print("webrtcbin create succeed");

    g_object_set(m_webrtcbin, "rtcp-rsize", TRUE, NULL);
    g_object_set(m_webrtcbin, "rtcp-rr-mode", 1, NULL);      // 启用RTCP反馈
    g_object_set(m_webrtcbin, "jitterbuffer-mode", 1, NULL); // 启用抖动缓冲

    //为webrtcbin设置bundle策略属性，值为max-bundle意思为尽可能将多个媒体流打包到单个的连接中，以减少网络延迟和带宽
    gst_util_set_object_arg(G_OBJECT(m_webrtcbin), "bundle-policy", "max-bundle");

    //将元素添加到管道中去
    gst_bin_add_many(GST_BIN(m_pipeline), m_audio_bin, m_video_bin, m_webrtcbin, NULL);

    if (!gst_element_link(m_audio_bin, m_webrtcbin))
        gst_printerr("Failed to link m_audio_bin with m_webrtcbin \n");
    if (!gst_element_link(m_video_bin, m_webrtcbin))
        gst_printerr("Failed to link m_video bin with m_webrtcbin \n");
    /************************************************************  
    //如果我是作为answer 即create_offer为false{}
    
    //如果我是作为offer,考虑是否写头部扩展
*************************************************************/

    g_signal_connect(m_webrtcbin,
                     "on-negotiation-needed",
                     G_CALLBACK(on_negotiation_needed),
                     GINT_TO_POINTER(create_offer));
    g_signal_connect(m_webrtcbin, "on-ice-candidate", G_CALLBACK(send_ice_candidate_message), NULL);

    //监听总线事件
    bus = gst_pipeline_get_bus(GST_PIPELINE(m_pipeline));
    gst_bus_add_watch(bus, bus_watch_cb, m_pipeline);
    gst_object_unref(bus);

    //将管道状态设置为ready
    gst_element_set_state(m_pipeline, GST_STATE_READY);

    //将webrtcbin元素的pad-added信号与处理媒体流的回调函数相连，动态添加元素decodebin
    g_signal_connect(m_webrtcbin, "pad-added", G_CALLBACK(on_incoming_stream), m_pipeline);

    gst_print("Starting pipline\n");
    ret = gst_element_set_state(GST_ELEMENT(m_pipeline), GST_STATE_PLAYING);
    if (ret == GST_STATE_CHANGE_FAILURE) {
        g_print("start pipeline error");
        if (m_pipeline)
            g_clear_object(&m_pipeline);
        if (m_webrtcbin)
            m_webrtcbin = NULL;
        return FALSE;
    }
    return TRUE;
}

void PiplineBuild::on_negotiation_needed(GstElement *element, gpointer user_data)
{
    gboolean create_offer = GPOINTER_TO_INT(user_data);
    app_state = PEER_CALL_NEGOTIATING;

    //本地创建offer
    if (create_offer) {
        //设置promise异步操作完成后的回调函数
        GstPromise *promise = gst_promise_new_with_change_func(on_offer_created, NULL, NULL);
        //m_webrtcbin发送信号"create_offer",并将promise作为参数传递，当m_webrtcbin异步创建offer完成后调用promise的回调函数
        g_signal_emit_by_name(m_webrtcbin, "create-offer", NULL, promise);
    }

    /***********************************************************
    //远端创建offer,请求远端传教offer
    if(!create_offer){
        
    }
************************************************************/
}

//当offer创建完成后作为promise的回调函数调用，提取offer,设置本地描述并向服务器发送sdp
void PiplineBuild::on_offer_created(GstPromise *promise, gpointer user_data)
{
    GstWebRTCSessionDescription *offer = NULL;
    const GstStructure *reply;

    g_assert_cmphex(app_state, ==, PEER_CALL_NEGOTIATING);
    g_assert_cmphex(gst_promise_wait(promise), ==, GST_PROMISE_RESULT_REPLIED);
    //提取offer
    reply = gst_promise_get_reply(promise);
    gst_structure_get(reply, "offer", GST_TYPE_WEBRTC_SESSION_DESCRIPTION, &offer, NULL);
    gst_promise_unref(promise);

    // //打印sdp
    // gchar *sdp = gst_sdp_message_as_text(offer->sdp);
    // g_print("Offer SDP:\n%s\n", sdp);
    // g_free(sdp);

    // 设置本地描述
    GstPromise *local_promise = gst_promise_new();
    g_signal_emit_by_name(m_webrtcbin, "set-local-description", offer, local_promise);
    gst_promise_interrupt(local_promise);
    gst_promise_unref(local_promise);

    //向信令服务器发送offer
    send_sdp_to_peer(offer);
    gst_webrtc_session_description_free(offer);
}

//将offer或answer转为json数据发送给服务器
void PiplineBuild::send_sdp_to_peer(GstWebRTCSessionDescription *desc)
{
    gchar *text;
    JsonObject *msg, *sdp;

    text = gst_sdp_message_as_text(desc->sdp);

    sdp = json_object_new();
    if (desc->type == GST_WEBRTC_SDP_TYPE_OFFER) {
        gst_print("Sending offer to signaling server:\n%s\n", text);
        json_object_set_string_member(sdp, "type", "offer");
    } else if (desc->type == GST_WEBRTC_SDP_TYPE_ANSWER) {
        gst_print("Sending answer to signaling server:\n%s\n", text);
        json_object_set_string_member(sdp, "type", "answer");
    } else {
        g_assert_not_reached();
    }

    json_object_set_string_member(sdp, "sdp", text);
    g_free(text);

    msg = json_object_new();
    json_object_set_object_member(msg, "sdp", sdp);
    text = get_string_from_json_object(msg);
    std::string data = text;
    json_object_unref(msg);

    /*****************************************/
    SendMsg::GetInstance()->SendRequest(data, m_object_id, MSG_VIDEO_CHAT);
    sleep(1);
    /*向服务器发送消息的函数*******************/
    g_free(text);
    text = NULL;
}
//将json对象转为gchar
gchar *PiplineBuild::get_string_from_json_object(JsonObject *object)
{
    JsonNode *root;
    JsonGenerator *generator;
    gchar *text;

    root = json_node_init_object(json_node_alloc(), object);
    generator = json_generator_new();
    json_generator_set_root(generator, root);
    text = json_generator_to_data(generator, NULL);

    g_object_unref(generator);
    json_node_free(root);
    return text;
}

//向信令服务器发送ice-candidate候选者
void PiplineBuild::send_ice_candidate_message(GstElement *m_webrtcbin,
                                              guint mlineindex,
                                              gchar *candidate,
                                              gpointer user_data)
{
    gchar *text;
    JsonObject *ice, *msg;

    ice = json_object_new();
    json_object_set_string_member(ice, "candidate", candidate);
    json_object_set_int_member(ice, "sdpMLineIndex", mlineindex);
    msg = json_object_new();
    json_object_set_object_member(msg, "ice", ice);
    text = get_string_from_json_object(msg);

    g_print("this is ice-candidate:%s\n", text);

    json_object_unref(msg);
    std::string data = text;
    /**********************************************/
    SendMsg::GetInstance()->SendRequest(data, m_object_id, MSG_VIDEO_CHAT);
    sleep(1);
    /* * 向信令服务器发送候选者text*******************/
    g_free(text);
    text = NULL;
}
gboolean PiplineBuild::bus_watch_cb(GstBus *bus, GstMessage *message, gpointer user_data)
{
    GstPipeline *pipeline = static_cast<GstPipeline *>(user_data);

    switch (GST_MESSAGE_TYPE(message)) {
    case GST_MESSAGE_ASYNC_DONE: {
        GST_DEBUG_BIN_TO_DOT_FILE_WITH_TS(GST_BIN(m_pipeline),
                                          GST_DEBUG_GRAPH_SHOW_ALL,
                                          "webrtc-sendrecv.async-done");
        break;
    }
    case GST_MESSAGE_ERROR: {
        GError *error = NULL;
        gchar *debug = NULL;

        GST_DEBUG_BIN_TO_DOT_FILE_WITH_TS(GST_BIN(m_pipeline),
                                          GST_DEBUG_GRAPH_SHOW_ALL,
                                          "webrtc-sendrecv.error");

        gst_message_parse_error(message, &error, &debug);
        g_printerr("Error on bus: %s (debug: %s)", message->src, error->message);
        g_printerr("Debugging info: %s\n", (debug) ? debug : "none");
        g_error_free(error);
        g_free(debug);
        cleanup_and_quit_loop("bus have some erro", APP_STATE_UNKNOWN);
        break; 
    }
    case GST_MESSAGE_WARNING: {
        GError *error = NULL;
        gchar *debug = NULL;

        gst_message_parse_warning(message, &error, &debug);
        g_warning("Warning on bus: %s (debug: %s)", error->message, debug);
        g_error_free(error);
        g_free(debug);
        break;
    }
    case GST_MESSAGE_LATENCY:
        gst_bin_recalculate_latency(GST_BIN(pipeline));
        break;
    default:
        break;
    }
    return G_SOURCE_CONTINUE;
}


//清理
gboolean PiplineBuild::cleanup_and_quit_loop(const char *msg, enum AppState state)
{
    //在这里处理一下要是通道接收到错误信号怎么清理并退出程序
    /*****************...**********************/
    if (msg) {
        gst_printerr("cleanup_and_quit_loop %s\n", msg);
    }

    if (loop) {
        g_main_loop_quit(loop);
        g_clear_pointer(&loop, g_main_loop_unref);
    }

    /* To allow usage as a GSourceFunc */
    return G_SOURCE_REMOVE;
}

//webrtcbin元素的pad-added信号的回调函数，将webrtcbin的pad与创建的decodebin的pad连接
void PiplineBuild::on_incoming_stream(GstElement *webrtc, GstPad *pad, GstElement *pipe)
{
    g_print("this is on_incoming_stream function");
    GstElement *decodebin;
    GstPad *sinkpad;

    if (GST_PAD_DIRECTION(pad) != GST_PAD_SRC) //检查pad是否是source方向
        return;

    decodebin = gst_element_factory_make("decodebin", NULL);
    g_signal_connect(decodebin, "pad-added", G_CALLBACK(on_incoming_decodebin_stream), pipe);
    gst_bin_add(GST_BIN(pipe), decodebin);
    gst_element_sync_state_with_parent(decodebin);

    sinkpad = gst_element_get_static_pad(decodebin, "sink");
    gst_pad_link(pad, sinkpad);
    gst_object_unref(sinkpad);
}

//判断传入媒体流的类型，根据不同类型的流将不同的参数传给媒体流处理函数
void PiplineBuild::on_incoming_decodebin_stream(GstElement *decodebin, GstPad *pad, GstElement *pipe)
{
    GstCaps *caps;
    const gchar *name;

    if (!gst_pad_has_current_caps(pad)) {
        gst_printerr("pad '%s' has no caps,can't do anything,ignoring\n", GST_PAD_NAME(pad));
        return;
    }
    caps = gst_pad_get_current_caps(pad);
    name = gst_structure_get_name(gst_caps_get_structure(caps, 0));

    gst_print("the caps name:%s", name);
    if (g_str_has_prefix(name, "video")) {
        handle_media_stream(pad, pipe, "videoconvert", "xvimagesink");
    } else if (g_str_has_prefix(name, "audio")) {
        handle_media_stream(pad, pipe, "audioconvert", "autoaudiosink");
    } else {
        gst_printerr("Unknown pad %s", GST_PAD_NAME(pad));
    }
}

//媒体流处理函数，将媒体流与剩下的管道元素连接，处理媒体流
void PiplineBuild::handle_media_stream(GstPad *pad,
                                       GstElement *pipe,
                                       const char *convert_name,
                                       const char *sink_name)
{
    GstPad *qpad;
    GstElement *q, *conv, *resample, *sink;
    GstPadLinkReturn ret;

    gst_println("Tring to handle streame with %s ！ %s", convert_name, sink_name);

    q = gst_element_factory_make("queue", NULL);
    g_assert_nonnull(q);
    conv = gst_element_factory_make(convert_name, NULL);
    g_assert_nonnull(conv);
    sink = gst_element_factory_make(sink_name, NULL);
    g_assert_nonnull(sink);

    if (g_strcmp0(convert_name, "audioconvert") == 0) {
        resample = gst_element_factory_make("audioresample", NULL);
        g_assert_nonnull(resample);
        gst_bin_add_many(GST_BIN(pipe), q, conv, resample, sink, NULL);
        gst_element_sync_state_with_parent(q);
        gst_element_sync_state_with_parent(conv);
        gst_element_sync_state_with_parent(resample);
        gst_element_sync_state_with_parent(sink);
        gst_element_link_many(q, conv, sink, NULL);
    } else {
        gst_bin_add_many(GST_BIN(pipe), q, conv, sink, NULL);
        gst_element_sync_state_with_parent(q);
        gst_element_sync_state_with_parent(conv);
        gst_element_sync_state_with_parent(sink);
        gst_element_link_many(q, conv, sink, NULL);
    }
    qpad = gst_element_get_static_pad(q, "sink");
    ret = gst_pad_link(pad, qpad);
    g_assert_cmphex(ret, ==, GST_PAD_LINK_OK);
}

//创建answer后将answer设置为本地描述，并发送给远端
void PiplineBuild::on_answer_create(GstPromise *promise, gpointer user_data)
{
    GstWebRTCSessionDescription *answer = NULL;
    const GstStructure *reply;

    g_assert_cmphex(app_state, ==, PEER_CALL_NEGOTIATING);

    g_assert_cmphex(gst_promise_wait(promise), ==, GST_PROMISE_RESULT_REPLIED);
    reply = gst_promise_get_reply(promise);
    gst_structure_get(reply, "answer", GST_TYPE_WEBRTC_SESSION_DESCRIPTION, &answer, NULL);
    gst_promise_unref(promise);

    promise = gst_promise_new();
    g_signal_emit_by_name(m_webrtcbin, "set-local-description", answer, promise);
    gst_promise_interrupt(promise);
    gst_promise_unref(promise);

    send_sdp_to_peer(answer);
    gst_webrtc_session_description_free(answer);
}

void PiplineBuild::on_offer_set(GstPromise *promise, gpointer user_data)
{
    gst_print("remote set succeed!!!!!!!!!!!!!!!");
    gst_promise_unref(promise);
    promise = gst_promise_new_with_change_func(on_answer_create, NULL, NULL);
    g_signal_emit_by_name(m_webrtcbin, "create-answer", NULL, promise);
}

void PiplineBuild::on_offer_received(GstSDPMessage *sdp)
{
    GstWebRTCSessionDescription *offer = NULL;
    GstPromise *promise;

    gst_print("Starting pipeline as answer!");

    // if (!start_pipeline(is_offer)) {
    //     gst_print("Start pipeline failed");
    // }

    offer = gst_webrtc_session_description_new(GST_WEBRTC_SDP_TYPE_OFFER, sdp);
    g_assert_nonnull(offer);

    promise = gst_promise_new_with_change_func(on_offer_set, NULL, NULL);
    g_signal_emit_by_name(m_webrtcbin, "set-remote-description", offer, promise);

    gst_webrtc_session_description_free(offer);
}
