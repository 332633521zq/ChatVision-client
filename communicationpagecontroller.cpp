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
    messages.clear();
    setMssages();
    for (auto &it : messages) {
        QString text = it["text"].toString();
        QString sender = it["sender"].toString();
        if (sender == m_myId) {
            setMyMessage(text);
        } else {
            setFriendMessage(text);
        }
    }
}

void CommunicationPageController::saveMessage()
{
    QJsonObject jo;
    QString path = "/root/test/" + m_friendId + ".json";
    jo["text"] = m_myMessage;
    jo["sender"] = m_myId;
    if (!Tool::getInstance().saveJsonObjectToFile(jo, path)) {
        qDebug() << "save failed";
    }
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
