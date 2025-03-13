#include "findfriendpagecontroller.h"

FindFriendPageController &FindFriendPageController::getInstance()
{
    static FindFriendPageController ffc;
    return ffc;
}

FindFriendPageController::FindFriendPageController(QObject *parent)
    : QObject{parent}
{}

void FindFriendPageController::initFindFriendPage()
{
    QString name = "cat1";
    QString avater = "qrc:/image/avater.jpg";
    QString friendId = "2000008";
    emit initFindPage(name, avater, friendId);
}
