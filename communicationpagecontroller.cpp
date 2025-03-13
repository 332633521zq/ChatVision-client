#include "communicationpagecontroller.h"

CommunicationPageController::CommunicationPageController(QObject *parent)
    : QObject(parent)
{}

CommunicationPageController &CommunicationPageController::getInstance()
{
    static CommunicationPageController cpc;
    return cpc;
}

void CommunicationPageController::setMssages()
{
    QString path = "/root/test/" + m_friendId + ".json";
    Tool::getInstance().getJsonFromFile(messages, path);
}
void CommunicationPageController::initCommunicationPage()
{
    unsigned int object_id = m_friendId.toUInt();
    json totalmsg = FileTools::GetInstance()->GetLatestMsg(object_id);
    if (totalmsg.size() == 0) {
        return;
    }
    for (const auto &json_it : totalmsg) {
        std::cout << "initCommunication msg is:" << json_it << std::endl;
        // json json_it = json::parse(it);
        QString text = QString::fromStdString(json_it["data"].get<std::string>());
        QString uid = QString::number(json_it["uid"].get<unsigned int>());
        if (uid == m_myId) {
            setMyMessage(text);
        } else {
            setFriendMessage(text);
        }
    }
}

void CommunicationPageController::saveMessage()
{
    json jo;
    jo["data"] = m_myMessage.toStdString();
    jo["uid"] = m_myId.toUInt();
    unsigned int object_id = m_friendId.toUInt();
    jo["object_id"] = object_id;
    if (!FileTools::GetInstance()->SaveTextMsg(object_id, jo)) {
        qDebug() << "save send msg in file failed";
    }
}

void CommunicationPageController::sendMessage(QString data)
{
    std::string s_data = data.toStdString();
    unsigned int object_id = m_friendId.toUInt();
    SendMsg::GetInstance()->SendRequest("", object_id, MSG_FOLLOWING);
    SendMsg::GetInstance()->SendRequest(s_data, object_id, MSG_TEXT_CHAT);
}

void CommunicationPageController::callRequest()
{
    unsigned int object_id = m_friendId.toUInt();
    PiplineBuild::m_object_id = object_id;
    MediaThread *mediaThread = new MediaThread(true);
    mediaThread->startThread();

    // QObject::connect(mediaThread,
    //                  &MediaThread::wasHangUp,
    //                  &CommunicationPageController::getInstance(),
    //                  &CommunicationPageController::onWasHangUp);
    // SendMsg::GetInstance()->SendRequest("", object_id, MSG_VIDEO_CHAT);
}

void CommunicationPageController::getThrough()
{
    PiplineBuild::setPiplinePlaying();
}

void CommunicationPageController::hangUp()
{
    unsigned int object_id = m_friendId.toUInt();
    SendMsg::GetInstance()->SendRequest("HangUp", object_id, MSG_VIDEO_CHAT);
    PiplineBuild::cleanup_and_quit_loop("挂断", PEER_CALL_STOPPED);
}

void CommunicationPageController::saveMessage(QString msg, QString send_id)
{
    QJsonObject jo;
    QString path = "root/test" + send_id + ".json";
    jo["text"] = msg;
    jo["sender"] = send_id;
    if (!Tool::getInstance().saveJsonObjectToFile(jo, path)) {
        qDebug() << "save friend msg failed";
    }
    if (send_id == m_friendId) {
        setFriendMessage(msg);
    }
}

QString CommunicationPageController::myMessage() const
{
    return m_myMessage;
}

void CommunicationPageController::setMyMessage(const QString myMessage)
{
    m_myMessage = myMessage;
    emit myMessageChanged();
}

QString CommunicationPageController::friendMessage() const
{
    return m_friendMessage;
}

void CommunicationPageController::setFriendMessage(const QString friendMessage)
{
    m_friendMessage = friendMessage;
    emit friendMessageChanged();
}

QString CommunicationPageController::myId() const
{
    return m_myId;
}

void CommunicationPageController::setMyId(const QString myId)
{
    m_myId = myId;
}

QString CommunicationPageController::friendId() const
{
    return m_friendId;
}

void CommunicationPageController::setFriendId(const QString friendId)
{
    qDebug() << "friend id was set" << friendId;
    m_friendId = friendId;
}

void CommunicationPageController::onWasHangUp()
{
    emit closeVideoWindow();
}
