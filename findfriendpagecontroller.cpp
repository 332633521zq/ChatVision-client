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
        // std::cout << "it is:" << key << std::endl;
        QString area = QString::fromStdString(value["area"]);
        qDebug() << "!!!!!!!!!!!!!!!" << area;
        QString avater = QString::fromStdString(value["avatar_path_"]);
        qDebug() << "!!!!!!!!!!!!!!!" << avater;

        QString gender = QString::fromStdString(value["gender"]);
        QString memo = QString::fromStdString(value["memo"]);
        QString nickname = QString::fromStdString(value["nickname"]);
        QString signature = QString::fromStdString(value["signature"]);
        QString uid = QString::fromStdString(value["uid"]);
        emit initFindPage(area, gender, memo, nickname, signature, uid, avater);
    }
}

void FindFriendPageController::sendRandowRequest()
{
    unsigned int uid = User::GetInstance()->GetUid();
    SendMsg::GetInstance()->SendRequest("", uid, MSG_RANDOM_PUSH);
}

void FindFriendPageController::addFocus(QString obid)
{
    qDebug() << "obid is:" << obid;
    unsigned int object_id = obid.toUInt();
    SendMsg::GetInstance()->SendRequest("", object_id, MSG_FOLLOWING);
}
