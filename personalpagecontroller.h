#pragma once

#include <QDebug>
#include <QFileInfo>
#include <QObject>
#include <QQmlEngine>
#include <QStandardPaths>
#include "msgsender.h"
#include "user.h"

class PersonalPageController : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString netNumber READ netNumber WRITE setNetNumber NOTIFY netNumberChanged)
    Q_PROPERTY(QString ipNumber READ ipNumber WRITE setIpNumber)

public:
    Q_INVOKABLE void init();

    static PersonalPageController &getInstance();
    void setNetNumber(const QString number);
    QString netNumber() const;
    void setIpNumber(const QString ipnumber);
    QString ipNumber() const;
signals:
    void netNumberChanged(const unsigned int netnumber);

private:
    unsigned int m_number;
    std::string m_ipNumber;
    PersonalPageController(QObject *parent = nullptr);
};
