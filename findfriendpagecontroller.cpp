#include "findfriendpagecontroller.h"
#include <QDebug>
FindFriendPageController &FindFriendPageController::getInstance()
{
    static FindFriendPageController ffc;
    return ffc;
}

FindFriendPageController::FindFriendPageController(QObject *parent)
    : QObject{parent}
{}

void FindFriendPageController::initFindFriendPage(json &jsondata)
{
    for (auto &[key, value] : jsondata.items()) {
        QString area = QString::fromStdString(value["area"]);
        QString avater = QString::fromStdString(value["avatar_path_"]);
        QString gender = QString::fromStdString(value["gender"]);
        QString memo = QString::fromStdString(value["memo"]);
        QString nickname = QString::fromStdString(value["nickname"]);
        QString signature = QString::fromStdString(value["signature"]);
        QString uid = QString::fromStdString(value["uid"]);
        QString relation = QString::fromStdString(value["relation"]);
        emit initFindPage(area, gender, memo, nickname, signature, uid, avater, relation);
    }
}

void FindFriendPageController::sendRandowRequest()
{
    unsigned int uid = User::GetInstance()->GetUid();
    MsgSender::GetInstance()->SendRequest("", uid, MSG_RANDOM_PUSH);
}

void FindFriendPageController::addFocus(QString obid)
{
    qDebug() << "obid is:" << obid;
    unsigned int object_id = obid.toUInt();
    MsgSender::GetInstance()->SendRequest("", object_id, MSG_FOLLOWING);
}
