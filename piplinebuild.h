#pragma once
#include <gst/gst.h>
#include <gst/gststructure.h>
#include <gst/rtp/rtp.h>
#include <gst/sdp/sdp.h>
#include <gst/webrtc/nice/nice.h>
#include <gst/webrtc/webrtc.h>
#include <json-glib/json-glib.h>

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
    gboolean start_pipeline(gboolean create_offer);

    static void send_ice_candidate_message(GstElement *webrtc G_GNUC_UNUSED,
                                           guint mlineindex,
                                           gchar *candidate,
                                           gpointer user_data G_GNUC_UNUSED);

    static void on_negotiation_needed(GstElement *element, gpointer user_data);
    static void on_offer_created(GstPromise *promise, gpointer user_data);
    static void send_sdp_to_peer(GstWebRTCSessionDescription *desc);
    static gchar *get_string_from_json_object(JsonObject *object);
    static gboolean bus_watch_cb(GstBus *bus, GstMessage *message, gpointer user_data);

private:
    static GstElement *m_pipeline, *m_webrtcbin, *m_audio_bin, *m_video_bin;
    static enum AppState app_state;
    GObject *send_channel, *receive_channel;
};
