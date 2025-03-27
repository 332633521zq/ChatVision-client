#include "searchcontroller.h"
#include <QDebug>
SearchController::SearchController(QObject *parent)
    : QObject{parent}
{
    connect(this, &SearchController::searchIDChanged, this, &SearchController::onSearchIDChanged);
}

SearchController &SearchController::getInstance()
{
    static SearchController sc;
    return sc;
}

QString SearchController::searchID() const
{
    return m_searchID;
}

void SearchController::setSearchID(const QString sID)
{
    m_searchID = sID;
    qDebug() << "m_searchID is " << m_searchID;
    emit searchIDChanged();
}

void SearchController::searchUser()
{
    unsigned int objectid = m_searchID.toUInt();
    SendMsg::GetInstance()->SendRequest("", objectid, MSG_SEARCH);
}

void SearchController::onSearchIDChanged()
{
    // bool ret;
    // std::map<unsigned int, json> ts;
    // ts = User::GetInstance()->GetChatted();
    // std::cout << "ts.begin is" << ts.size() << std::endl;
    // for (auto it = ts.begin(); it != ts.end(); ++it) {
    //     std::cout << it->first << " " << it->second << std::endl;
    //     ret = ((it->first).find(m_searchID) != std::string::npos);
    //     if(ret){

    //     }
    // }
}
