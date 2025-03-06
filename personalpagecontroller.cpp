#include "personalpagecontroller.h"

PersonalPageController &PersonalPageController::getInstance()
{
    static PersonalPageController ppc;
    return ppc;
}
PersonalPageController::PersonalPageController(QObject *parent)
    : QObject(parent)
{}

void PersonalPageController::setNetNumber(const QString number)
{
    bool ret;
    m_number = number.toUInt(&ret);
    if (!ret) {
        qDebug() << "netNumber convert to unsigned int failed int function setNetNumber";
    } else {
        qDebug() << "newnumber set succeed";
    }
    // emit netNumberChanged(m_number);
}

QString PersonalPageController::netNumber() const
{
    return QString::number(m_number);
}
void PersonalPageController::test()
{
    qDebug() << "this is test";
}
