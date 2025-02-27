#include "piplinebuild.h"
GstElement *PiplineBuild::m_audio_bin = nullptr;
GstElement *PiplineBuild::m_pipeline = nullptr;
GstElement *PiplineBuild::m_video_bin = nullptr;
GstElement *PiplineBuild::m_webrtcbin = nullptr;
enum AppState PiplineBuild::app_state = APP_STATE_UNKNOWN;
PiplineBuild::PiplineBuild() {}
//创建管道
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

    audio_desc = g_strdup_printf(
        "autoaudiosrc! audioconvert ! audioresample"
        "! queue ! opusenc perfect-timestamp=true ! rtpopuspay name=audiopay pt=%u "
        "! application/x-rtp, encoding-name=OPUS ! queue",
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

    video_desc = g_strdup_printf("v4l2src  ! videoconvert ! queue ! "
                                 "vp8enc deadline=1 keyframe-max-dist=2000 ! "
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
    m_webrtcbin = gst_element_factory_make_full("webrtcbin",
                                                "name",
                                                "sendrecv",
                                                "stun-server",
                                                STUN_SERVER,
                                                NULL);
    g_assert_nonnull(m_webrtcbin);
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

    g_signal_emit_by_name(m_webrtcbin, "create-data-channel", "channel", NULL, &send_channel);
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

    //打印sdp
    gchar *sdp = gst_sdp_message_as_text(offer->sdp);
    g_print("Offer SDP:\n%s\n", sdp);
    g_free(sdp);

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
        json_object_set_string_member(sdp, "type", "offer");
    } else {
        g_assert_not_reached();
    }

    json_object_set_string_member(sdp, "sdp", text);
    g_free(text);

    msg = json_object_new();
    json_object_set_object_member(msg, "sdp", sdp);
    text = get_string_from_json_object(msg);
    json_object_unref(msg);

    /*****************************************
 * 向服务器发送消息的函数*******************/
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
    json_object_unref(msg);

    /**********************************************
     * 向信令服务器发送候选者text*******************/
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
        g_warning("Error on bus: %s (debug: %s)", error->message, debug);
        g_error_free(error);
        g_free(debug);
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
