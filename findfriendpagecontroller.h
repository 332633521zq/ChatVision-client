#pragma once
#include <QObject>
#include "msgsender.h"
#include "user.h"
class FindFriendPageController : public QObject
{
    Q_OBJECT
public:
    static FindFriendPageController &getInstance();
    Q_INVOKABLE void initFindFriendPage(json &jsondata);
    Q_INVOKABLE void sendRandowRequest();

private:
    explicit FindFriendPageController(QObject *parent = nullptr);

signals:
    void initFindPage(QString area,
                      // QString avater,
                      QString gender,
                      QString memo,
                      QString nickname,
                      QString signature,
                      QString uid);
};
