#pragma once
#include "ConstValue.h"
#include <boost/asio.hpp>
#include <gst/gst.h>
#include <gst/gststructure.h>
#include <gst/rtp/rtp.h>
#include <gst/sdp/sdp.h>
#include <gst/webrtc/nice/nice.h>
#include <gst/webrtc/webrtc.h>
#include <iostream>
#include <json-glib/json-glib.h>
#include <json/json.h>
#include <json/reader.h>
#include <json/value.h>
#include <nlohmann/json.hpp>

const int MAX_LENGTH = 1024 * 2;

#define RTP_OPUS_DEFAULT_PT 97
#define RTP_VP8_DEFAULT_PT 96
#define STUN_SERVER "stun://stun.l.google.com:19302"
enum AppState {
    APP_STATE_UNKNOWN = 0,
    APP_STATE_ERROR = 1, /* generic error */
    SERVER_CONNECTING = 1000,
    SERVER_CONNECTION_ERROR,
    SERVER_CONNECTED, /* Ready to register */
    SERVER_REGISTERING = 2000,
    SERVER_REGISTRATION_ERROR,
    SERVER_REGISTERED, /* Ready to call a peer */
    SERVER_CLOSED,     /* server connection closed by us or the server */
    PEER_CONNECTING = 3000,
    PEER_CONNECTION_ERROR,
    PEER_CONNECTED,
    PEER_CALL_NEGOTIATING = 4000,
    PEER_CALL_STARTED,
    PEER_CALL_STOPPING,
    PEER_CALL_STOPPED,
    PEER_CALL_ERROR,
};

class PiplineBuild
{
public:
    PiplineBuild();
    //创建管道
    static gboolean start_pipeline(gboolean create_offer);

    static void send_ice_candidate_message(GstElement *webrtc G_GNUC_UNUSED,
                                           guint mlineindex,
                                           gchar *candidate,
                                           gpointer user_data G_GNUC_UNUSED);

    static void on_negotiation_needed(GstElement *element, gpointer user_data);
    static void on_offer_created(GstPromise *promise, gpointer user_data);
    static void send_sdp_to_peer(GstWebRTCSessionDescription *desc);
    static gchar *get_string_from_json_object(JsonObject *object);
    static gboolean bus_watch_cb(GstBus *bus, GstMessage *message, gpointer user_data);
    static gboolean cleanup_and_quit_loop(const char *msg, enum AppState state);
    static void on_incoming_stream(GstElement *webrtc, GstPad *pad, GstElement *pipe);
    static void on_incoming_decodebin_stream(GstElement *decodebin, GstPad *pad, GstElement *pipe);
    static void handle_media_stream(GstPad *pad,
                                    GstElement *pipe,
                                    const char *convert_name,
                                    const char *sink_name);
    static void on_answer_create(GstPromise *promise, gpointer user_data);
    static void on_offer_set(GstPromise *promise, gpointer user_data);
    static void on_offer_received(GstSDPMessage *sdp);
    static boost::asio::ip::tcp::socket *m_socket;
    static unsigned int m_object_id;
    static enum AppState app_state;
    static GstElement *m_webrtcbin;
    static GMainLoop *loop;

private:
    static GstElement *m_pipeline, *m_audio_bin, *m_video_bin;

    static GObject *send_channel,
        *receive_channel; //send_channel是由本地创建，receive_channel是由远程创建
    static gboolean is_offer;
};
