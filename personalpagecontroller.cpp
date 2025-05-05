#include "personalpagecontroller.h"
#include "client.h"

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

    User::GetInstance()->SetUid(m_number);

    // emit netNumberChanged(m_number);
}

QString PersonalPageController::netNumber() const
{
    return QString::number(m_number);
}
void PersonalPageController::setIpNumber(const QString number)
{
    // bool ret;
    std::cout << "this is set ip" << std::endl;
    m_ipNumber = number.toStdString();
    std::cout << m_ipNumber << std::endl;
    // if (!ret) {
    //     qDebug() << "netNumber convert to unsigned int failed int function setNetNumber";
    // } else {
    //     qDebug() << "newnumber set succeed";
    // }
    // emit netNumberChanged(m_number);
}

QString PersonalPageController::ipNumber() const
{
    return QString::fromStdString(m_ipNumber);
}
void PersonalPageController::init()
{
    Client::getInstance().setIpAddress(m_ipNumber);
    Client::getInstance().connectServer();
    std::string is_first_login = "yes";

    std::filesystem::path root_path
        = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation).toStdString();
    std::cout << "root_path:" << root_path;
    std::filesystem::path user_dir = root_path / "ChatVisionUserInfo" / std::to_string(m_number);

    QFileInfo fileInfo(user_dir);
    if (fileInfo.exists())
        is_first_login = "no";

    MsgSender::GetInstance()->SendRequest(is_first_login, m_number, MSG_LOGIN);
}
