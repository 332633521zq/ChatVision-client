#pragma once

#include <QDebug>
#include <QObject>
#include <QQmlEngine>
#include "sendmsg.h"
#include "user.h"
class PersonalPageController : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString netNumber READ netNumber WRITE setNetNumber NOTIFY netNumberChanged)
public:
    Q_INVOKABLE void init();

    static PersonalPageController &getInstance();
    void setNetNumber(const QString number);
    QString netNumber() const;
signals:
    void netNumberChanged(const unsigned int netnumber);

private:
    unsigned int m_number;
    PersonalPageController(QObject *parent = nullptr);
};
