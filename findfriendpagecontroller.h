#pragma once
#include <QObject>
class FindFriendPageController : public QObject
{
    Q_OBJECT
public:
    static FindFriendPageController &getInstance();
    Q_INVOKABLE void initFindFriendPage();

private:
    explicit FindFriendPageController(QObject *parent = nullptr);

signals:
    void initFindPage(QString name, QString age, QString friendId);
};
