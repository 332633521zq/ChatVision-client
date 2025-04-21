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
#include "msgsender.h"
#include "tool.h"
#include "user.h"

class CommunicationPageController : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString myMessage READ myMessage WRITE setMyMessage NOTIFY myMessageChanged FINAL)
    Q_PROPERTY(QString friendMessage READ friendMessage WRITE setFriendMessage NOTIFY
                   friendMessageChanged FINAL)
    Q_PROPERTY(QString myId READ myId WRITE setMyId NOTIFY myIdChanged FINAL)
    Q_PROPERTY(QString friendId READ friendId WRITE setFriendId NOTIFY msgDateChanged FINAL)
    Q_PROPERTY(QString msgDate READ GetMsgDate WRITE setMsgDate NOTIFY friendIdChanged FINAL)
    Q_PROPERTY(QList<QJsonObject> history_msgs READ GetHistoryMsgs NOTIFY historyMsgChanged FINAL)

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
    Q_INVOKABLE void initChattedList();
    Q_INVOKABLE void initMsgDate();

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

    QList<QJsonObject> GetHistoryMsgs() const;

    QString GetMsgDate() const;
    void setMsgDate(const QString datetime);

    void addDayMsg(const QString datetime);
signals:
    void myMessageChanged();
    void friendMessageChanged();
    void myIdChanged();
    void friendIdChanged();
    void videoCallRequest();
    void closeVideoWindow();
    void agreeCall();
    void addListElement(QString id,
                        QString memo,
                        QString nickname,
                        QString area,
                        QString gender,
                        QString signature,
                        QString avatar_path);
    void historyMsgChanged();
    void msgDateChanged();

public slots:
    void onWasHangUp();

private:
    QString m_myMessage;
    QString m_friendMessage;
    QString m_myId;
    QString m_friendId;
    QVector<QJsonObject> messages;
    QString m_msg_date;
    QList<QJsonObject> m_history_msgs;
    CommunicationPageController(QObject *parent = nullptr);
};
