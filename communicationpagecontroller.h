#pragma once

#include <QCoreApplication>
#include <QDebug>
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QObject>
#include <QVector>
#include "filetools.h"
#include "mediathread.h"
#include "sendmsg.h"
#include "tool.h"

class CommunicationPageController : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString myMessage READ myMessage WRITE setMyMessage NOTIFY myMessageChanged FINAL)
    Q_PROPERTY(QString friendMessage READ friendMessage WRITE setFriendMessage NOTIFY
                   friendMessageChanged FINAL)
    Q_PROPERTY(QString myId READ myId WRITE setMyId NOTIFY myIdChanged FINAL)
    Q_PROPERTY(QString friendId READ friendId WRITE setFriendId NOTIFY friendIdChanged FINAL)
public:
    static CommunicationPageController &getInstance();
    void setMssages();
    Q_INVOKABLE void initCommunicationPage();
    //保存我发送的消息
    Q_INVOKABLE void saveMessage();
    Q_INVOKABLE void sendMessage(QString data);
    Q_INVOKABLE void callRequest();
    Q_INVOKABLE void getThrough();
    Q_INVOKABLE void hangUp();
    //保存我接收的消息，需要消息内容和发送者id
    void saveMessage(QString msg, QString send_id);
    QString myMessage() const;
    void setMyMessage(const QString myMessage);

    QString friendMessage() const;
    void setFriendMessage(const QString friendMessage);

    QString myId() const;
    void setMyId(const QString myId);

    QString friendId() const;
    void setFriendId(const QString friendId);

signals:
    void myMessageChanged();
    void friendMessageChanged();
    void myIdChanged();
    void friendIdChanged();
    void videoCallRequest();
    void closeVideoWindow();
public slots:
    void onWasHangUp();

private:
    QString m_myMessage;
    QString m_friendMessage;
    QString m_myId;
    QString m_friendId;
    QVector<QJsonObject> messages;
    CommunicationPageController(QObject *parent = nullptr);
};
